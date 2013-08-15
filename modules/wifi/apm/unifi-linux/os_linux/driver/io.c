/*
 * ---------------------------------------------------------------------------
 *  FILE:     io.c
 *
 *  PURPOSE:
 *      This file contains routines that the SDIO driver can call when a
 *      UniFi card is first inserted (or detected) and removed.
 *
 *      When used with sdioemb, the udev scripts (at least on Ubuntu) don't
 *      recognise a UniFi being added to the system. This is because sdioemb
 *      does not register itself as a device_driver, it uses it's own code
 *      to handle insert and remove.
 *      To have Ubuntu recognise UniFi, edit /etc/udev/rules.d/85-ifupdown.rules
 *      to change this line:
 *          SUBSYSTEM=="net", DRIVERS=="?*", GOTO="net_start"
 *      to these:
 *          #SUBSYSTEM=="net", DRIVERS=="?*", GOTO="net_start"
 *          SUBSYSTEM=="net", GOTO="net_start"
 *
 *      Then you can add a stanza to /etc/network/interfaces like this:
 *          auto eth1
 *          iface eth1 inet dhcp
 *          wpa-conf /etc/wpa_supplicant.conf
 *      This will then automatically associate when a car dis inserted.
 *
 * Copyright (C) 2006-2009 by Cambridge Silicon Radio Ltd.
 *
 * Refer to LICENSE.txt included with this source code for details on
 * the license terms.
 *
 * ---------------------------------------------------------------------------
 */
#include <linux/proc_fs.h>

#include <driver/unifi.h>
#include <driver/unifiversion.h>
#include <driver/unifi_udi.h>   /* for unifi_print_status() */
#include "unifiio.h"
#include "unifi_priv.h"


/*
 * Array of pointers to context structs for unifi devices that are present.
 * The index in the array corresponds to the wlan interface number
 * (if "wlan*" is used). If "eth*" is used, the eth* numbers are allocated
 * after any Ethernet cards.
 *
 * The Arasan PCI-SDIO controller card supported by this driver has 2 slots,
 * hence a max of 2 devices.
 */
static unifi_priv_t *Unifi_instances[MAX_UNIFI_DEVS];

/*
 * Array to hold the status of each unifi device in each slot.
 * We only process an insert event when In_use[] for the slot is
 * UNIFI_DEV_NOT_IN_USE. Otherwise, it means that the slot is in use or
 * we are in the middle of a cleanup (the action on unplug).
 */
#define UNIFI_DEV_NOT_IN_USE    0
#define UNIFI_DEV_IN_USE        1
#define UNIFI_DEV_CLEANUP       2
static int In_use[MAX_UNIFI_DEVS];
/*
 * Mutex to prevent UDI clients to open the character device before the priv
 * is created and initialised.
 */
DECLARE_MUTEX(Unifi_instance_mutex);
/*
 * When the device is removed, unregister waits on Unifi_cleanup_wq
 * until all the UDI clients release the character device.
 */
DECLARE_WAIT_QUEUE_HEAD(Unifi_cleanup_wq);


static int uf_read_proc(char *page, char **start, off_t offset, int count,
                        int *eof, void *data);



/*
 * ---------------------------------------------------------------------------
 *  uf_register_netdev
 *
 *      Registers the network interface, installes the qdisc,
 *      and registers the inet handler.
 *      In the porting exercise, register the driver to the network
 *      stack if necessary.
 *
 *  Arguments:
 *      priv          Pointer to driver context.
 *
 *  Returns:
 *      O on success, non-zero otherwise.
 *
 *  Notes:
 *      We will only unregister when the card is ejected, so we must
 *      only do it once.
 * ---------------------------------------------------------------------------
 */
int
uf_register_netdev(unifi_priv_t *priv)
{
    int r;

    /*
     * Allocates a device number and registers device with the network
     * stack.
     */
    r = register_netdev(priv->netdev);
    if (r) {
        unifi_error(priv, "Failed to register net device\n");
        return -EINVAL;
    }

    /* The device is registed */
    priv->netdev_registered = 1;

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,28)
#ifdef CONFIG_NET_SCHED
    /*
     * IMPORTANT:
     * uf_install_qdisc() holds the network device lock, we can not
     * install the qdisk before the network device is registered.
     */
    r = uf_install_qdisc(priv->netdev);
    if (r) {
        unifi_error(priv, "Failed to install qdisc\n");
        return r;
    }
#endif /* CONFIG_NET_SCHED */
#endif /* LINUX_VERSION_CODE */

#if (defined CSR_SUPPORT_SME) && (defined CSR_SUPPORT_WEXT)
    /*
     * Register the inet handler; it notifies us for changes in the IP address.
     */
    uf_register_inet_notifier();
#endif /* CSR_SUPPORT_SME && CSR_SUPPORT_WEXT */

    unifi_notice(priv, "unifi%d is %s\n",
                 priv->instance, priv->netdev->name);

    return 0;
} /* uf_register_netdev */


/*
 * ---------------------------------------------------------------------------
 *  uf_unregister_netdev
 *
 *      Unregisters the network interface and the inet handler.
 *
 *  Arguments:
 *      priv          Pointer to driver context.
 *
 *  Returns:
 *      None.
 *
 * ---------------------------------------------------------------------------
 */
static void
uf_unregister_netdev(unifi_priv_t *priv)
{
    if (priv->netdev_registered) {
#if (defined CSR_SUPPORT_SME) && (defined CSR_SUPPORT_WEXT)
        /* Unregister the inet handler... */
        uf_unregister_inet_notifier();
#endif /* CSR_SUPPORT_SME && CSR_SUPPORT_WEXT */

        /* ... and the netdev */
        unifi_trace(priv, UDBG5, "cleanup_unifi_sdio: unregister_netdev\n");
        unregister_netdev(priv->netdev);
        priv->netdev_registered = 0;
    }

} /* uf_unregister_netdev() */


/*
 * ---------------------------------------------------------------------------
 *  register_unifi_sdio
 *
 *      This function is called from the Probe (or equivalent) method of
 *      the SDIO driver when a UniFi card is detected.
 *      We allocate the Linux net_device struct, initialise the HIP core
 *      lib, create the char device nodes and start the userspace helper
 *      to initialise the device.
 *
 *  Arguments:
 *      sdio_dev        Pointer to SDIO context handle to use for all
 *                      SDIO ops.
 *      bus_id          A small number indicating the SDIO card position on the
 *                      bus. Typically this is the slot number, e.g. 0, 1 etc.
 *                      Valid values are 0 to MAX_UNIFI_DEVS-1.
 *      dev             Pointer to kernel device manager struct.
 *
 *  Returns:
 *      Pointer to the unifi instance, or NULL on error.
 * ---------------------------------------------------------------------------
 */
static unifi_priv_t *
register_unifi_sdio(CsrSdioFunction *sdio_dev, int bus_id, struct device *dev)
{
    unifi_priv_t *priv = NULL;
    int r = -1;

    func_enter();

    if ((bus_id < 0) || (bus_id >= MAX_UNIFI_DEVS)) {
        unifi_error(priv, "register_unifi_sdio: invalid device %d\n",
                    bus_id);
        return NULL;
    }

    down(&Unifi_instance_mutex);

    if (In_use[bus_id] != UNIFI_DEV_NOT_IN_USE) {
        unifi_error(priv, "register_unifi_sdio: device %d is already in use\n",
                    bus_id);
        goto failed0;
    }


    /* Allocate device private and net_device structs */
    priv = uf_alloc_netdevice(sdio_dev, bus_id);
    if (priv == NULL) {
        unifi_error(priv, "Failed to allocate driver private\n");
        goto failed0;
    }

    priv->unifi_device = dev;

    SET_NETDEV_DEV(priv->netdev, dev);

    /* We are not ready to send data yet. */
    netif_carrier_off(priv->netdev);

    /* Allocate driver context. */
    priv->card = unifi_alloc_card(priv->sdio, priv);
    if (priv->card == NULL) {
        unifi_error(priv, "Failed to allocate UniFi driver card struct.\n");
        goto failed1;
    }

    if (Unifi_instances[bus_id]) {
        unifi_error(priv, "Internal error: instance for slot %d is already taken\n",
                    bus_id);
    }
    Unifi_instances[bus_id] = priv;
    In_use[bus_id] = UNIFI_DEV_IN_USE;

    /* Create the character device nodes */
    r = uf_create_device_nodes(priv, bus_id);
    if (r) {
        goto failed1;
    }

    /*
     * We use the slot number as unifi device index.
     */
    snprintf(priv->proc_entry_name, 64, "driver/unifi%d", priv->instance);
    if (!create_proc_read_entry(priv->proc_entry_name, 0, 0,
                                uf_read_proc, (void *)priv->instance))
    {
        unifi_error(priv, "unifi: can't create /proc/driver/unifi\n");
    }

    /* Initialise the SME related threads and parameters */
    r = uf_sme_init(priv);
    if (r) {
        unifi_error(priv, "SME initialisation failed.\n");
        goto failed2;
    }


    /*
     * Run the userspace helper program (unififw) to perform
     * the device initialisation.
     */
    unifi_trace(priv, UDBG1, "run UniFi helper app...\n");
    r = uf_run_unifihelper(priv);
    if (r) {
        unifi_notice(priv, "unable to run UniFi helper app\n");
        /* Not a fatal error. */
    }

    up(&Unifi_instance_mutex);

    func_exit();
    return priv;


failed2:
    /* Remove the device nodes */
    uf_destroy_device_nodes(priv);
failed1:
    /* Deregister priv->netdev_client */
    ul_deregister_client(priv->netdev_client);

failed0:
    if (priv && priv->card) {
        unifi_free_card(priv->card);
    }
    if (priv) {
        uf_free_netdevice(priv);
    }

    up(&Unifi_instance_mutex);

    func_exit();
    return NULL;
} /* register_unifi_sdio() */


/*
 * ---------------------------------------------------------------------------
 *  ask_unifi_sdio_cleanup
 *
 *      We can not free our private context, until all the char device
 *      clients have closed the file handles. unregister_unifi_sdio() which
 *      is called when a card is removed, waits on Unifi_cleanup_wq until
 *      the reference count becomes zero. It is time to wake it up now.
 *
 *  Arguments:
 *      priv          Pointer to driver context.
 *
 *  Returns:
 *      None.
 * ---------------------------------------------------------------------------
 */
static void
ask_unifi_sdio_cleanup(unifi_priv_t *priv)
{
    func_enter();

    /*
     * Now clear the flag that says the old instance is in use.
     * This is used to prevent a new instance being started before old
     * one has finshed closing down, for example if bounce makes the card
     * appear to be ejected and re-inserted quickly.
     */
    In_use[priv->instance] = UNIFI_DEV_CLEANUP;

    unifi_trace(NULL, UDBG5, "ask_unifi_sdio_cleanup: wake up cleanup workqueue.\n");
    wake_up(&Unifi_cleanup_wq);

    func_exit();

} /* ask_unifi_sdio_cleanup() */


/*
 * ---------------------------------------------------------------------------
 *  cleanup_unifi_sdio
 *
 *      Release any resources owned by a unifi instance.
 *
 *  Arguments:
 *      priv          Pointer to the instance to free.
 *
 *  Returns:
 *      None.
 * ---------------------------------------------------------------------------
 */
static void
cleanup_unifi_sdio(unifi_priv_t *priv)
{
    int priv_instance;
    static const unifi_MACAddress broadcast_address = {{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}};

    func_enter();

    /* Remove the device nodes */
    uf_destroy_device_nodes(priv);

    /* Mark this device as gone away by NULLing the entry in Unifi_instances */
    Unifi_instances[priv->instance] = NULL;

    unifi_trace(priv, UDBG5, "cleanup_unifi_sdio: remove_proc_entry\n");
    /*
     * Free the children of priv before unifi_free_netdevice() frees
     * the priv struct
     */
    remove_proc_entry(priv->proc_entry_name, 0);

    /* Unregister netdev as a client. */
    if (priv->netdev_client) {
        unifi_trace(priv, UDBG2, "Netdev client (id:%d s:0x%X) is unregistered\n",
                    priv->netdev_client->client_id, priv->netdev_client->sender_id);
        ul_deregister_client(priv->netdev_client);
    }

    /* Destroy the SME related threads and parameters */
    uf_sme_deinit(priv);

#ifdef CSR_SME_USERSPACE
    priv->smepriv = NULL;
#endif

    /* Free any packets left in the Rx queues */
    uf_free_pending_rx_packets(priv, UF_UNCONTROLLED_PORT_Q, broadcast_address);
    uf_free_pending_rx_packets(priv, UF_CONTROLLED_PORT_Q, broadcast_address);

    /*
     * We need to free the resources held by the core, which include tx skbs,
     * otherwise we can not call unregister_netdev().
     */
    if (priv->card) {
        unifi_trace(priv, UDBG5, "cleanup_unifi_sdio: free card\n");
        unifi_free_card(priv->card);
        priv->card = NULL;
    }

    /*
     * Unregister the network device.
     * We can not unregister the netdev before we release
     * all pending packets in the core.
     */
    uf_unregister_netdev(priv);

    unifi_trace(priv, UDBG5, "cleanup_unifi_sdio: uf_free_netdevice\n");
    /*
     * When uf_free_netdevice() returns, the priv is invalid
     * so we need to remember the instance to clear the global flag later.
     */
    priv_instance = priv->instance;

    /* Priv is freed as part of the net_device */
    uf_free_netdevice(priv);

    /*
     * Now clear the flag that says the old instance is in use.
     * This is used to prevent a new instance being started before old
     * one has finshed closing down, for example if bounce makes the card
     * appear to be ejected and re-inserted quickly.
     */
    In_use[priv_instance] = UNIFI_DEV_NOT_IN_USE;

    unifi_trace(NULL, UDBG5, "cleanup_unifi_sdio: DONE.\n");

    func_exit();

} /* cleanup_unifi_sdio() */


/*
 * ---------------------------------------------------------------------------
 *  unregister_unifi_sdio
 *
 *      Call from SDIO driver when it detects that UniFi has been removed.
 *
 *  Arguments:
 *      bus_id          Number of the card that was ejected.
 *
 *  Returns:
 *      None.
 * ---------------------------------------------------------------------------
 */
static void
unregister_unifi_sdio(int bus_id)
{
    unifi_priv_t *priv;
    u8 reason = CONFIG_IND_EXIT;

    if ((bus_id < 0) || (bus_id >= MAX_UNIFI_DEVS)) {
        unifi_error(NULL, "unregister_unifi_sdio: invalid device %d\n",
                    bus_id);
        return;
    }

    priv = Unifi_instances[bus_id];
    if (priv == NULL) {
        unifi_error(priv, "unregister_unifi_sdio: device %d is not registered\n",
                    bus_id);
        func_exit();
        return;
    }

    /* Stop the network traffic before freeing the core. */
    netif_carrier_off(priv->netdev);
    UF_NETIF_TX_STOP_ALL_QUEUES(priv->netdev);

#ifdef CSR_NATIVE_LINUX
    /*
     * If the unifi thread was started, signal it to stop.  This
     * should cause any userspace processes with open unifi device to
     * close them.
     */
    uf_stop_thread(priv, &priv->bh_thread);

    /* Unregister the interrupt handler */
    if (csr_sdio_linux_remove_irq(priv->sdio)) {
        unifi_notice(priv,
                     "csr_sdio_linux_remove_irq failed to talk to card.\n");
    }

    /* Ensure no MLME functions are waiting on a the mlme_event semaphore. */
    uf_abort_mlme(priv);
#endif /* CSR_NATIVE_LINUX */

    ul_log_config_ind(priv, &reason, sizeof(u8));

    /* Deregister the UDI hook from the core. */
    unifi_remove_udi_hook(priv->card, logging_handler);

    uf_put_instance(bus_id);

    /*
     * Wait until the device is cleaned up. i.e., when all userspace
     * processes have closed any open unifi devices.
     */
    wait_event(Unifi_cleanup_wq, In_use[bus_id] == UNIFI_DEV_CLEANUP);
    unifi_trace(NULL, UDBG5, "Received clean up event\n");

    /* Now we can free the private context and the char device nodes */
    cleanup_unifi_sdio(priv);

} /* unregister_unifi_sdio() */


/*
 * ---------------------------------------------------------------------------
 *  uf_find_instance
 *
 *      Find the context structure for a given UniFi device instance.
 *
 *  Arguments:
 *      inst            The instance number to look for.
 *
 *  Returns:
 *      None.
 * ---------------------------------------------------------------------------
 */
unifi_priv_t *
uf_find_instance(int inst)
{
    if ((inst < 0) || (inst >= MAX_UNIFI_DEVS)) {
        return NULL;
    }
    return Unifi_instances[inst];
} /* uf_find_instance() */


/*
 * ---------------------------------------------------------------------------
 *  uf_find_priv
 *
 *      Find the device instance for a given context structure.
 *
 *  Arguments:
 *      priv            The context structure pointer to look for.
 *
 *  Returns:
 *      index of instance, -1 otherwise.
 * ---------------------------------------------------------------------------
 */
int
uf_find_priv(unifi_priv_t *priv)
{
    int inst;

    if (!priv) {
        return -1;
    }

    for (inst = 0; inst < MAX_UNIFI_DEVS; inst++) {
        if (Unifi_instances[inst] == priv) {
            return inst;
        }
    }

    return -1;
} /* uf_find_priv() */

/*
 * ---------------------------------------------------------------------------
 *  uf_get_instance
 *
 *      Find the context structure for a given UniFi device instance
 *      and increment the reference count.
 *
 *  Arguments:
 *      inst            The instance number to look for.
 *
 *  Returns:
 *      Pointer to the instance or NULL if no instance exists.
 * ---------------------------------------------------------------------------
 */
unifi_priv_t *
uf_get_instance(int inst)
{
    unifi_priv_t *priv;

    down(&Unifi_instance_mutex);

    priv = uf_find_instance(inst);
    if (priv) {
        priv->ref_count++;
    }

    up(&Unifi_instance_mutex);

    return priv;
}

/*
 * ---------------------------------------------------------------------------
 *  uf_put_instance
 *
 *      Decrement the context reference count, freeing resources and
 *      shutting down the driver when the count reaches zero.
 *
 *  Arguments:
 *      inst            The instance number to look for.
 *
 *  Returns:
 *      Pointer to the instance or NULL if no instance exists.
 * ---------------------------------------------------------------------------
 */
void
uf_put_instance(int inst)
{
    unifi_priv_t *priv;

    down(&Unifi_instance_mutex);

    priv = uf_find_instance(inst);
    if (priv) {
        priv->ref_count--;
        if (priv->ref_count == 0) {
            ask_unifi_sdio_cleanup(priv);
        }
    }

    up(&Unifi_instance_mutex);
}


/*
 * ---------------------------------------------------------------------------
 *  uf_read_proc
 *
 *      Read method for driver node in /proc/driver/unifi0
 *
 *  Arguments:
 *      page
 *      start
 *      offset
 *      count
 *      eof
 *      data
 *
 *  Returns:
 *      None.
 * ---------------------------------------------------------------------------
 */
#ifdef CONFIG_PROC_FS
static int
uf_read_proc(char *page, char **start, off_t offset, int count,
                int *eof, void *data)
{
    unifi_priv_t *priv;
    char *p = page;
    int i;
#ifdef CSR_SME_EMB
    int r;
    unifi_Versions sme_versions;
#endif

    priv = uf_find_instance((int)data);
    if (!priv) {
        return 0;
    }

    if (offset == 0) {
        p += sprintf(p, "UniFi SDIO Driver: v%s (build:%d) %s %s\n",
                     UNIFI_DRIVER_VERSION, UNIFI_DRIVER_BUILD_ID,
                     __DATE__, __TIME__);
#ifdef CSR_SME_EMB
        r = sme_mgt_get_versions(priv, &sme_versions);
        if (r == 0) {
            p += sprintf(p, "SME: CSR embedded build:%lu\n", sme_versions.smeBuild);
        } else {
            p += sprintf(p, "SME: CSR embedded build:unknown\n");
        }
#endif
#ifdef CSR_SME_USERSPACE
        p += sprintf(p, "SME: CSR userspace ");
#ifdef CSR_SUPPORT_WEXT
        p += sprintf(p, "with WEXT support\n");
#else
        p += sprintf(p, "\n");
#endif /* CSR_SUPPORT_WEXT */
#endif /* CSR_SME_USERSPACE */
#ifdef CSR_NATIVE_LINUX
        p += sprintf(p, "SME: native\n");
#endif

#ifdef CSR_SUPPORT_SME
        p += sprintf(p, "Firmware (ROM) build:%lu, Patch:%lu\n",
                     priv->card_info.fw_build, priv->sme_versions.firmwarePatch);
#endif
        p += unifi_print_status(priv->card, p);

        p += sprintf(p, "Last dbg str: %s\n", priv->last_debug_string);

        p += sprintf(p, "Last dbg16:");
        for (i = 0; i < 8; i++) {
            p += sprintf(p, " %04X", priv->last_debug_word16[i]);
        }
        p += sprintf(p, "\n");
        p += sprintf(p, "           ");
        for (; i < 16; i++) {
            p += sprintf(p, " %04X", priv->last_debug_word16[i]);
        }
        p += sprintf(p, "\n");
    } else {
        *eof = 1;
    }

    return (p - page);
} /* uf_read_proc() */
#endif




static CsrInt32
uf_lx_suspend(CsrSdioFunction *sdio_ctx)
{
    unifi_priv_t *priv = sdio_ctx->driverData;
    unifi_suspend(priv);

    return 0;
}

static CsrInt32
uf_lx_resume(CsrSdioFunction *sdio_ctx)
{
    unifi_priv_t *priv = sdio_ctx->driverData;
    unifi_resume(priv);

    return 0;
}

static int active_slot = MAX_UNIFI_DEVS;
static struct device *os_devices[MAX_UNIFI_DEVS];

void
uf_add_os_device(int bus_id, struct device *os_device)
{
    if ((bus_id < 0) || (bus_id >= MAX_UNIFI_DEVS)) {
        unifi_error(NULL, "uf_add_os_device: invalid device %d\n",
                    bus_id);
        return;
    }

    active_slot = bus_id;
    os_devices[bus_id] = os_device;
} /* uf_add_os_device() */

void
uf_remove_os_device(int bus_id)
{
    if ((bus_id < 0) || (bus_id >= MAX_UNIFI_DEVS)) {
        unifi_error(NULL, "uf_remove_os_device: invalid device %d\n",
                    bus_id);
        return;
    }

    active_slot = bus_id;
    os_devices[bus_id] = NULL;
} /* uf_remove_os_device() */

static CsrInt32
uf_sdio_inserted(CsrSdioFunction *sdio_ctx)
{
    unifi_priv_t *priv;

    unifi_trace(NULL, UDBG5, "uf_sdio_inserted(0x%p), slot_id=%d, dev=%p\n",
                sdio_ctx, active_slot, os_devices[active_slot]);

    priv = register_unifi_sdio(sdio_ctx, active_slot, os_devices[active_slot]);
    if (priv == NULL) {
        return -ENOMEM;
    }

    sdio_ctx->driverData = priv;

    return 0;
} /* uf_sdio_inserted() */


static void
uf_sdio_removed(CsrSdioFunction *sdio_ctx)
{
    unregister_unifi_sdio(active_slot);
} /* uf_sdio_removed() */


static void
uf_sdio_dsr_handler(CsrSdioFunction *sdio_ctx, CsrInt32 result)
{
    unifi_priv_t *priv = sdio_ctx->driverData;

    unifi_sdio_interrupt_handler(priv->card);
} /* uf_sdio_dsr_handler() */

/*
 * ---------------------------------------------------------------------------
 *  uf_sdio_int_handler
 *
 *      Interrupt callback function for SDIO interrupts.
 *      This is called in kernel context (i.e. not interrupt context).
 *      We retrieve the unifi context pointer and call the main UniFi
 *      interrupt handler.
 *
 *  Arguments:
 *      fdev      SDIO context pointer
 *
 *  Returns:
 *      None.
 * ---------------------------------------------------------------------------
 */
static CsrSdioDsrCallback
uf_sdio_int_handler(CsrSdioFunction *sdio_ctx)
{
    return uf_sdio_dsr_handler;
} /* uf_sdio_int_handler() */




static CsrSdioFunctionId unifi_ids[] =
{
    {
        .manfId = SDIO_MANF_ID_CSR,
        .cardId = SDIO_CARD_ID_UNIFI_3,
        .sdioFunction = SDIO_WLAN_FUNC_ID_UNIFI_3,
        .sdioInterface = CSR_SDIO_ANY_SDIO_INTERFACE,
    },
    {
        .manfId = SDIO_MANF_ID_CSR,
        .cardId = SDIO_CARD_ID_UNIFI_4,
        .sdioFunction = SDIO_WLAN_FUNC_ID_UNIFI_4,
        .sdioInterface = CSR_SDIO_ANY_SDIO_INTERFACE,
    }
};


/*
 * Structure to register with the glue layer.
 */
static CsrSdioFunctionDriver unifi_sdioFunction_drv =
{
    .inserted = uf_sdio_inserted,
    .removed = uf_sdio_removed,
    .interrupt = uf_sdio_int_handler,
    .suspend = uf_lx_suspend,
    .resume = uf_lx_resume,

    .ids = unifi_ids,
    .idsCount = sizeof(unifi_ids) / sizeof(unifi_ids[0])
};


/*
 * ---------------------------------------------------------------------------
 *  uf_sdio_load
 *  uf_sdio_unload
 *
 *      These functions are called from the main module load and unload
 *      functions. They perform the appropriate operations for the monolithic
 *      driver.
 *
 *  Arguments:
 *      None.
 *
 *  Returns:
 *      None.
 * ---------------------------------------------------------------------------
 */
int __init
uf_sdio_load(void)
{
    CsrInt32 csr_r;

    csr_r = CsrSdioFunctionDriverRegister(&unifi_sdioFunction_drv);
    if (csr_r) {
        unifi_error(NULL, "Failed to register UniFi SDIO driver: %d\n", csr_r);
        return convert_csr_error(csr_r);
    }

    return 0;
} /* uf_sdio_load() */



void __exit
uf_sdio_unload(void)
{
    CsrSdioFunctionDriverUnregister(&unifi_sdioFunction_drv);
} /* uf_sdio_unload() */

