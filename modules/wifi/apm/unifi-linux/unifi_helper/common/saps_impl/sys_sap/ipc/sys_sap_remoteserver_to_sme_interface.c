/* This is an autogenerated file */
/* Tag: noCheckHeader */

/*    CONFIDENTIAL */
/*    Copyright (C) Cambridge Silicon Radio Ltd 2008. All rights reserved. */

#include "event_pack_unpack/event_pack_unpack.h"
#include "fsm/fsm_internal.h"
#include "sme_top_level_fsm/sme_top_level_fsm.h"

static void event_to_unifi_sys_capabilities_cfm(FsmContext* context, CsrUint8* inputbuffer, CsrUint16 length)
{
    CsrUint8* buffer = &inputbuffer[6];
    UnifiSysCapabilitiesCfm_Evt evt;

    evt.appHandle = (void*) event_unpack_CsrUint32(&buffer);
    evt.commandQueueSize =  event_unpack_CsrUint16(&buffer);
    evt.trafficQueueSize =  event_unpack_CsrUint16(&buffer);

    unifi_sys_capabilities_cfm(context, evt.appHandle, evt.commandQueueSize, evt.trafficQueueSize);

}

static void event_to_unifi_sys_eapol_cfm(FsmContext* context, CsrUint8* inputbuffer, CsrUint16 length)
{
    CsrUint8* buffer = &inputbuffer[6];
    UnifiSysEapolCfm_Evt evt;

    evt.appHandle = (void*) event_unpack_CsrUint32(&buffer);
    evt.result = (unifi_EapolRc) event_unpack_CsrUint16(&buffer);

    unifi_sys_eapol_cfm(context, evt.appHandle, evt.result);

}

static void event_to_unifi_sys_hip_ind(FsmContext* context, CsrUint8* inputbuffer, CsrUint16 length)
{
    CsrUint8* buffer = &inputbuffer[6];
    UnifiSysHipInd_Evt evt;

    evt.mlmeCommandLength =  event_unpack_CsrUint16(&buffer);
    evt.mlmeCommand = NULL;
    if (evt.mlmeCommandLength)
    {
        evt.mlmeCommand = (CsrUint8*)CsrPmalloc(evt.mlmeCommandLength);
        event_unpack_buffer(&buffer, evt.mlmeCommand, evt.mlmeCommandLength);
    }
    evt.dataRef1Length =  event_unpack_CsrUint16(&buffer);
    evt.dataRef1 = NULL;
    if (evt.dataRef1Length)
    {
        evt.dataRef1 = (CsrUint8*)CsrPmalloc(evt.dataRef1Length);
        event_unpack_buffer(&buffer, evt.dataRef1, evt.dataRef1Length);
    }
    evt.dataRef2Length =  event_unpack_CsrUint16(&buffer);
    evt.dataRef2 = NULL;
    if (evt.dataRef2Length)
    {
        evt.dataRef2 = (CsrUint8*)CsrPmalloc(evt.dataRef2Length);
        event_unpack_buffer(&buffer, evt.dataRef2, evt.dataRef2Length);
    }

    unifi_sys_hip_ind(context, evt.mlmeCommandLength, evt.mlmeCommand, evt.dataRef1Length, evt.dataRef1, evt.dataRef2Length, evt.dataRef2);

    CsrPfree(evt.mlmeCommand);
    CsrPfree(evt.dataRef1);
    CsrPfree(evt.dataRef2);
}

static void event_to_unifi_sys_ip_configured_ind(FsmContext* context, CsrUint8* inputbuffer, CsrUint16 length)
{
    CsrUint8* buffer = &inputbuffer[6];
    UnifiSysIpConfiguredInd_Evt evt;

    evt.ipConfigured = (CsrBool) event_unpack_CsrUint16(&buffer);

    unifi_sys_ip_configured_ind(context, evt.ipConfigured);

}

static void event_to_unifi_sys_m4_transmitted_ind(FsmContext* context, CsrUint8* inputbuffer, CsrUint16 length)
{
    unifi_sys_m4_transmitted_ind(context);

}

static void event_to_unifi_sys_ma_unitdata_ind(FsmContext* context, CsrUint8* inputbuffer, CsrUint16 length)
{
    CsrUint8* buffer = &inputbuffer[6];
    UnifiSysMaUnitdataInd_Evt evt;

    evt.appHandle = (void*) event_unpack_CsrUint32(&buffer);
    evt.subscriptionHandle =  event_unpack_CsrUint8(&buffer);
    evt.frameLength =  event_unpack_CsrUint16(&buffer);
    evt.frame = NULL;
    if (evt.frameLength)
    {
        evt.frame = (CsrUint8*)CsrPmalloc(evt.frameLength);
        event_unpack_buffer(&buffer, evt.frame, evt.frameLength);
    }
    evt.freeFunction = NULL;
    buffer+=4;
    evt.receptionStatus = (unifi_ReceptionStatus) event_unpack_CsrUint16(&buffer);
    evt.priority = (unifi_Priority) event_unpack_CsrUint16(&buffer);
    evt.serviceClass = (unifi_ServiceClass) event_unpack_CsrUint16(&buffer);

    unifi_sys_ma_unitdata_ind(context, evt.appHandle, evt.subscriptionHandle, evt.frameLength, evt.frame, evt.freeFunction, evt.receptionStatus, evt.priority, evt.serviceClass);

    CsrPfree(evt.frame);
}

static void event_to_unifi_sys_ma_unitdata_cfm(FsmContext* context, CsrUint8* inputbuffer, CsrUint16 length)
{
    CsrUint8* buffer = &inputbuffer[6];
    UnifiSysMaUnitdataCfm_Evt evt;

    evt.appHandle = (void*) event_unpack_CsrUint32(&buffer);
    evt.result = (unifi_Status) event_unpack_CsrUint8(&buffer);
    evt.transmissionStatus = (unifi_TransmissionStatus) event_unpack_CsrUint16(&buffer);
    evt.providedPriority = (unifi_Priority) event_unpack_CsrUint16(&buffer);
    evt.providedServiceClass = (unifi_ServiceClass) event_unpack_CsrUint16(&buffer);
    evt.reqIdentifier =  event_unpack_CsrUint32(&buffer);

    unifi_sys_ma_unitdata_cfm(context, evt.appHandle, evt.result, evt.transmissionStatus, evt.providedPriority, evt.providedServiceClass, evt.reqIdentifier);

}

static void event_to_unifi_sys_ma_unitdata_subscribe_cfm(FsmContext* context, CsrUint8* inputbuffer, CsrUint16 length)
{
    CsrUint8* buffer = &inputbuffer[6];
    UnifiSysMaUnitdataSubscribeCfm_Evt evt;

    evt.appHandle = (void*) event_unpack_CsrUint32(&buffer);
    evt.subscriptionHandle =  event_unpack_CsrUint8(&buffer);
    evt.status = (unifi_SubscriptionResult) event_unpack_CsrUint8(&buffer);
    evt.allocOffset =  event_unpack_CsrUint16(&buffer);

    unifi_sys_ma_unitdata_subscribe_cfm(context, evt.appHandle, evt.subscriptionHandle, evt.status, evt.allocOffset);

}

static void event_to_unifi_sys_ma_unitdata_unsubscribe_cfm(FsmContext* context, CsrUint8* inputbuffer, CsrUint16 length)
{
    CsrUint8* buffer = &inputbuffer[6];
    UnifiSysMaUnitdataUnsubscribeCfm_Evt evt;

    evt.appHandle = (void*) event_unpack_CsrUint32(&buffer);
    evt.status = (unifi_SubscriptionResult) event_unpack_CsrUint8(&buffer);

    unifi_sys_ma_unitdata_unsubscribe_cfm(context, evt.appHandle, evt.status);

}

static void event_to_unifi_sys_multicast_address_ind(FsmContext* context, CsrUint8* inputbuffer, CsrUint16 length)
{
    CsrUint8* buffer = &inputbuffer[6];
    UnifiSysMulticastAddressInd_Evt evt;

    evt.action = (unifi_ListAction) event_unpack_CsrUint8(&buffer);
    evt.setAddressesCount =  event_unpack_CsrUint8(&buffer);
    evt.setAddresses = NULL;
    if (evt.setAddressesCount)
    {
        CsrUint16 i1;
        evt.setAddresses = (unifi_MACAddress*)CsrPmalloc(sizeof(unifi_MACAddress) * evt.setAddressesCount);
        for(i1 = 0; i1 < evt.setAddressesCount; i1++)
        {
            event_unpack_buffer(&buffer, evt.setAddresses[i1].data, 6);
        }
    }

    unifi_sys_multicast_address_ind(context, evt.action, evt.setAddressesCount, evt.setAddresses);

    CsrPfree(evt.setAddresses);
}

static void event_to_unifi_sys_port_configure_cfm(FsmContext* context, CsrUint8* inputbuffer, CsrUint16 length)
{
    CsrUint8* buffer = &inputbuffer[6];
    UnifiSysPortConfigureCfm_Evt evt;

    evt.status = (unifi_Status) event_unpack_CsrUint8(&buffer);
    event_unpack_buffer(&buffer, evt.macAddress.data, 6);

    unifi_sys_port_configure_cfm(context, evt.status, &evt.macAddress);

}

static void event_to_unifi_sys_qos_control_cfm(FsmContext* context, CsrUint8* inputbuffer, CsrUint16 length)
{
    CsrUint8* buffer = &inputbuffer[6];
    UnifiSysQosControlCfm_Evt evt;

    evt.status = (unifi_Status) event_unpack_CsrUint8(&buffer);

    unifi_sys_qos_control_cfm(context, evt.status);

}

static void event_to_unifi_sys_resume_ind(FsmContext* context, CsrUint8* inputbuffer, CsrUint16 length)
{
    CsrUint8* buffer = &inputbuffer[6];
    UnifiSysResumeInd_Evt evt;

    evt.powerMaintained = (CsrBool) event_unpack_CsrUint16(&buffer);

    unifi_sys_resume_ind(context, evt.powerMaintained);

}

static void event_to_unifi_sys_suspend_ind(FsmContext* context, CsrUint8* inputbuffer, CsrUint16 length)
{
    CsrUint8* buffer = &inputbuffer[6];
    UnifiSysSuspendInd_Evt evt;

    evt.hardSuspend = (CsrBool) event_unpack_CsrUint16(&buffer);
    evt.d3Suspend = (CsrBool) event_unpack_CsrUint16(&buffer);

    unifi_sys_suspend_ind(context, evt.hardSuspend, evt.d3Suspend);

}

static void event_to_unifi_sys_tclas_add_cfm(FsmContext* context, CsrUint8* inputbuffer, CsrUint16 length)
{
    CsrUint8* buffer = &inputbuffer[6];
    UnifiSysTclasAddCfm_Evt evt;

    evt.status = (unifi_Status) event_unpack_CsrUint8(&buffer);

    unifi_sys_tclas_add_cfm(context, evt.status);

}

static void event_to_unifi_sys_tclas_del_cfm(FsmContext* context, CsrUint8* inputbuffer, CsrUint16 length)
{
    CsrUint8* buffer = &inputbuffer[6];
    UnifiSysTclasDelCfm_Evt evt;

    evt.status = (unifi_Status) event_unpack_CsrUint8(&buffer);

    unifi_sys_tclas_del_cfm(context, evt.status);

}

static void event_to_unifi_sys_traffic_protocol_ind(FsmContext* context, CsrUint8* inputbuffer, CsrUint16 length)
{
    CsrUint8* buffer = &inputbuffer[6];
    UnifiSysTrafficProtocolInd_Evt evt;

    evt.packetType = (unifi_TrafficPacketType) event_unpack_CsrUint16(&buffer);
    evt.direction = (unifi_ProtocolDirection) event_unpack_CsrUint16(&buffer);
    event_unpack_buffer(&buffer, evt.srcAddress.data, 6);

    unifi_sys_traffic_protocol_ind(context, evt.packetType, evt.direction, &evt.srcAddress);

}

static void event_to_unifi_sys_traffic_sample_ind(FsmContext* context, CsrUint8* inputbuffer, CsrUint16 length)
{
    CsrUint8* buffer = &inputbuffer[6];
    UnifiSysTrafficSampleInd_Evt evt;

    evt.stats.rxMeanRate =  event_unpack_CsrUint32(&buffer);
    evt.stats.rxFramesNum =  event_unpack_CsrUint32(&buffer);
    evt.stats.txFramesNum =  event_unpack_CsrUint32(&buffer);
    evt.stats.rxBytesCount =  event_unpack_CsrUint32(&buffer);
    evt.stats.txBytesCount =  event_unpack_CsrUint32(&buffer);
    event_unpack_buffer(&buffer, evt.stats.intervals, 11);

    unifi_sys_traffic_sample_ind(context, &evt.stats);

}

static void event_to_unifi_sys_wifi_off_ind(FsmContext* context, CsrUint8* inputbuffer, CsrUint16 length)
{
    CsrUint8* buffer = &inputbuffer[6];
    UnifiSysWifiOffInd_Evt evt;

    evt.controlIndication = (unifi_ControlIndication) event_unpack_CsrUint8(&buffer);

    unifi_sys_wifi_off_ind(context, evt.controlIndication);

}

static void event_to_unifi_sys_wifi_off_cfm(FsmContext* context, CsrUint8* inputbuffer, CsrUint16 length)
{
    unifi_sys_wifi_off_cfm(context);

}

static void event_to_unifi_sys_wifi_on_ind(FsmContext* context, CsrUint8* inputbuffer, CsrUint16 length)
{
    CsrUint8* buffer = &inputbuffer[6];
    UnifiSysWifiOnInd_Evt evt;

    evt.status = (unifi_Status) event_unpack_CsrUint8(&buffer);
    evt.driverVersions.chipId =  event_unpack_CsrUint32(&buffer);
    evt.driverVersions.chipVersion =  event_unpack_CsrUint32(&buffer);
    evt.driverVersions.firmwareBuild =  event_unpack_CsrUint32(&buffer);
    evt.driverVersions.firmwareHip =  event_unpack_CsrUint32(&buffer);
    evt.driverVersions.driverBuild =  event_unpack_CsrUint32(&buffer);
    evt.driverVersions.driverHip =  event_unpack_CsrUint32(&buffer);

    unifi_sys_wifi_on_ind(context, evt.status, &evt.driverVersions);

}

static void event_to_unifi_sys_wifi_on_cfm(FsmContext* context, CsrUint8* inputbuffer, CsrUint16 length)
{
    CsrUint8* buffer = &inputbuffer[6];
    UnifiSysWifiOnCfm_Evt evt;

    evt.status = (unifi_Status) event_unpack_CsrUint8(&buffer);

    unifi_sys_wifi_on_cfm(context, evt.status);

}


typedef void (*event_to_fn)(FsmContext* context, CsrUint8* inputbuffer, CsrUint16 size);
static const event_to_fn fnlookup1[45] = {
        NULL,
        event_to_unifi_sys_wifi_on_ind,
        NULL,
        event_to_unifi_sys_wifi_on_cfm,
        NULL,
        event_to_unifi_sys_wifi_off_ind,
        NULL,
        event_to_unifi_sys_wifi_off_cfm,
        event_to_unifi_sys_suspend_ind,
        NULL,
        event_to_unifi_sys_resume_ind,
        NULL,
        NULL,
        event_to_unifi_sys_qos_control_cfm,
        NULL,
        event_to_unifi_sys_port_configure_cfm,
        NULL,
        NULL,
        event_to_unifi_sys_traffic_sample_ind,
        event_to_unifi_sys_traffic_protocol_ind,
        event_to_unifi_sys_ip_configured_ind,
        NULL,
        event_to_unifi_sys_multicast_address_ind,
        NULL,
        NULL,
        event_to_unifi_sys_tclas_add_cfm,
        NULL,
        event_to_unifi_sys_tclas_del_cfm,
        NULL,
        NULL,
        event_to_unifi_sys_m4_transmitted_ind,
        NULL,
        event_to_unifi_sys_hip_ind,
        NULL,
        event_to_unifi_sys_ma_unitdata_subscribe_cfm,
        NULL,
        event_to_unifi_sys_ma_unitdata_unsubscribe_cfm,
        NULL,
        event_to_unifi_sys_capabilities_cfm,
        NULL,
        event_to_unifi_sys_ma_unitdata_cfm,
        NULL,
        event_to_unifi_sys_ma_unitdata_ind,
        NULL,
        event_to_unifi_sys_eapol_cfm,

};



CsrBool remote_sys_signal_receive(FsmContext* context, CsrUint8* buffer, CsrUint16 size)
{
    CsrUint8* tempbuffer = buffer;
    CsrUint16 id = event_unpack_CsrUint16(&tempbuffer);

    if (id >= 0x0100 && id < 0x1000)
    {
        unifi_DataBlock mlmeCommand;
        unifi_DataBlock dataref1;
        unifi_DataBlock dataref2;

        mlmeCommand.length = size;
        mlmeCommand.data = buffer;

        tempbuffer += 6; /* Skip the 4 bytes for dest, sender and 1st slotnumber */
        dataref1.length = event_unpack_CsrUint16(&tempbuffer);
        tempbuffer += 2; /* Skip the slot number */
        dataref2.length = event_unpack_CsrUint16(&tempbuffer);

        dataref1.data = buffer + ((size - dataref2.length) - dataref1.length);
        dataref2.data = buffer +   size - dataref2.length;

        mlmeCommand.length -= dataref1.length + dataref2.length;
        unifi_sys_hip_ind(context, mlmeCommand.length, mlmeCommand.data, dataref1.length, dataref1.data, dataref2.length, dataref2.data);
        return TRUE;
    }

    if (id >= 0x8001 &&
        id <= 0x802D)
    {
        if (fnlookup1[id - 0x8001])
        {
            (*fnlookup1[id - 0x8001])(context, buffer, size);
            return TRUE;
        }
    }

    return FALSE;
}
