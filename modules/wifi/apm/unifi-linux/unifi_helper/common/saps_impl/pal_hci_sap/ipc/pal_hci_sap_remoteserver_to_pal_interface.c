/***************************************************************************
 *
 * This is an AUTOGENERATED file from genio_unpack_interface_c.pl
 *
 *    CONFIDENTIAL
 *    Copyright (C) Cambridge Silicon Radio Ltd 2007. All rights reserved.
 */

/***************************************************************************
 * Description:
 *       The following functions are used to unpack primitives, commands or
 *       in a C structure and send it to a choosen destination.
 *
 * Options used to generate this file:
 * perl genio_unpack_interface_c.pl
 *       -o /home/bfsw/host/releases/unifi_hostsoftware_212/build/unifi-driver-212/host/unifi_helper/unifi_sme/../common/saps_impl/pal_hci_sap/ipc/pal_hci_sap_remoteserver_to_pal_interface.c
 *       -x ../../common/hciio/../amp_hci/bt_amp_hci.xml
 *       -d down
 *       -c Opcode
 *       -i PAL_HCI
 *       -h HCI_Command_Header
 ***************************************************************************/

#include "fsm/csr_wifi_fsm.h" 
#include "fsm/fsm_internal.h" 
#include "sme_trace/sme_trace.h"
#include "event_pack_unpack/event_pack_unpack.h"
#include "sme_top_level_fsm/sme_top_level_fsm.h" 
#include "pal_hci_sap/pal_hci_sap.h" /* Specific definitions */
#include "pal_hci_sap/pal_hci_sap_signals.h" /* Autogenerated */
#include "pal_hci_sap/pal_hci_sap_unpack.h" 
#include "pal_hci_sap/pal_hci_sap_signals_trace.h" /* Autogenerated Trace */ 
static void send_hci_no_operation_cmd_unpacked(FsmContext *context, CsrUint8* inputbuffer, CsrUint16 size)
{
    trace_HciNoOperationCmd("", context, NULL);

    hci_no_operation_cmd(context);


}

static void send_hci_create_physical_link_cmd_unpacked(FsmContext *context, CsrUint8* inputbuffer, CsrUint16 size)
{
    CsrUint8* buffer = inputbuffer;
    HciCreatePhysicalLinkCmd *evt;
    CsrUint8 dedicatedAmpKeyLength;
    buffer = (inputbuffer+1); /* Move to the dedicatedAmpKeyLength */
    dedicatedAmpKeyLength = event_unpack_CsrUint8(&buffer); /* Dedicated_AMP_Key_Length */
    buffer = inputbuffer; /* Restore for normal decode */
    evt = (HciCreatePhysicalLinkCmd*)CsrPmalloc(sizeof(HciCreatePhysicalLinkCmd)+dedicatedAmpKeyLength);

    evt->physicalLinkHandle = event_unpack_CsrUint8(&buffer); /* Physical_Link_Handle */
    evt->dedicatedAmpKeyLength = event_unpack_CsrUint8(&buffer); /* Dedicated_AMP_Key_Length */
    evt->dedicatedAmpKeyType = event_unpack_CsrUint8(&buffer); /* Dedicated_AMP_Key_Type */
    evt->dedicatedAmpKey = (CsrUint8 *)(evt+1); /* Point to bottom of the structure to store var len data */
    event_unpack_buffer(&buffer, (CsrUint8*)evt->dedicatedAmpKey, 1 * evt->dedicatedAmpKeyLength); /*lint !e545 !e725    Dedicated_AMP_Key */
    trace_HciCreatePhysicalLinkCmd("", context, evt);

    hci_create_physical_link_cmd(context, evt->physicalLinkHandle, evt->dedicatedAmpKeyLength, evt->dedicatedAmpKeyType, evt->dedicatedAmpKey);

    CsrPfree(evt);

}

static void send_hci_accept_physical_link_request_cmd_unpacked(FsmContext *context, CsrUint8* inputbuffer, CsrUint16 size)
{
    CsrUint8* buffer = inputbuffer;
    HciAcceptPhysicalLinkRequestCmd *evt;
    CsrUint8 dedicatedAmpKeyLength;
    buffer = (inputbuffer+1); /* Move to the dedicatedAmpKeyLength */
    dedicatedAmpKeyLength = event_unpack_CsrUint8(&buffer); /* Dedicated_AMP_Key_Length */
    buffer = inputbuffer; /* Restore for normal decode */
    evt = (HciAcceptPhysicalLinkRequestCmd*)CsrPmalloc(sizeof(HciAcceptPhysicalLinkRequestCmd)+dedicatedAmpKeyLength);

    evt->physicalLinkHandle = event_unpack_CsrUint8(&buffer); /* Physical_Link_Handle */
    evt->dedicatedAmpKeyLength = event_unpack_CsrUint8(&buffer); /* Dedicated_AMP_Key_Length */
    evt->dedicatedAmpKeyType = event_unpack_CsrUint8(&buffer); /* Dedicated_AMP_Key_Type */
    evt->dedicatedAmpKey = (CsrUint8 *)(evt+1); /* Point to bottom of the structure to store var len data */
    event_unpack_buffer(&buffer, (CsrUint8*)evt->dedicatedAmpKey, 1 * evt->dedicatedAmpKeyLength); /*lint !e545 !e725    Dedicated_AMP_Key */
    trace_HciAcceptPhysicalLinkRequestCmd("", context, evt);

    hci_accept_physical_link_request_cmd(context, evt->physicalLinkHandle, evt->dedicatedAmpKeyLength, evt->dedicatedAmpKeyType, evt->dedicatedAmpKey);

    CsrPfree(evt);

}

static void send_hci_disconnect_physical_link_cmd_unpacked(FsmContext *context, CsrUint8* inputbuffer, CsrUint16 size)
{
    CsrUint8* buffer = inputbuffer;
    HciDisconnectPhysicalLinkCmd *evt = (HciDisconnectPhysicalLinkCmd*)CsrPmalloc(sizeof(HciDisconnectPhysicalLinkCmd));

    evt->physicalLinkHandle = event_unpack_CsrUint8(&buffer); /* Physical_Link_Handle */
    evt->reason = event_unpack_CsrUint8(&buffer); /* Reason */
    trace_HciDisconnectPhysicalLinkCmd("", context, evt);

    hci_disconnect_physical_link_cmd(context, evt->physicalLinkHandle, evt->reason);

    CsrPfree(evt);

}

static void send_hci_create_logical_link_cmd_unpacked(FsmContext *context, CsrUint8* inputbuffer, CsrUint16 size)
{
    CsrUint8* buffer = inputbuffer;
    HciCreateLogicalLinkCmd *evt = (HciCreateLogicalLinkCmd*)CsrPmalloc(sizeof(HciCreateLogicalLinkCmd));

    evt->physicalLinkHandle = event_unpack_CsrUint8(&buffer); /* Physical_Link_Handle */
    evt->txFlowSpec.id = event_unpack_CsrUint8(&buffer); /* id */
    evt->txFlowSpec.serviceType = event_unpack_CsrUint8(&buffer); /* Service_Type */
    evt->txFlowSpec.maximumSduSize = event_unpack_CsrUint16(&buffer); /* Maximum_SDU_Size */
    evt->txFlowSpec.sduInterArrivalTime = event_unpack_CsrUint32(&buffer); /* SDU_Inter_Arrival_Time */
    evt->txFlowSpec.accessLatency = event_unpack_CsrUint32(&buffer); /* Access_Latency */
    evt->txFlowSpec.flushTimeout = event_unpack_CsrUint32(&buffer); /* Flush_Timeout */
    evt->rxFlowSpec.id = event_unpack_CsrUint8(&buffer); /* id */
    evt->rxFlowSpec.serviceType = event_unpack_CsrUint8(&buffer); /* Service_Type */
    evt->rxFlowSpec.maximumSduSize = event_unpack_CsrUint16(&buffer); /* Maximum_SDU_Size */
    evt->rxFlowSpec.sduInterArrivalTime = event_unpack_CsrUint32(&buffer); /* SDU_Inter_Arrival_Time */
    evt->rxFlowSpec.accessLatency = event_unpack_CsrUint32(&buffer); /* Access_Latency */
    evt->rxFlowSpec.flushTimeout = event_unpack_CsrUint32(&buffer); /* Flush_Timeout */
    trace_HciCreateLogicalLinkCmd("", context, evt);

    hci_create_logical_link_cmd(context, evt->physicalLinkHandle, &evt->txFlowSpec, &evt->rxFlowSpec);

    CsrPfree(evt);

}

static void send_hci_accept_logical_link_cmd_unpacked(FsmContext *context, CsrUint8* inputbuffer, CsrUint16 size)
{
    CsrUint8* buffer = inputbuffer;
    HciAcceptLogicalLinkCmd *evt = (HciAcceptLogicalLinkCmd*)CsrPmalloc(sizeof(HciAcceptLogicalLinkCmd));

    evt->physicalLinkHandle = event_unpack_CsrUint8(&buffer); /* Physical_Link_Handle */
    evt->txFlowSpec.id = event_unpack_CsrUint8(&buffer); /* id */
    evt->txFlowSpec.serviceType = event_unpack_CsrUint8(&buffer); /* Service_Type */
    evt->txFlowSpec.maximumSduSize = event_unpack_CsrUint16(&buffer); /* Maximum_SDU_Size */
    evt->txFlowSpec.sduInterArrivalTime = event_unpack_CsrUint32(&buffer); /* SDU_Inter_Arrival_Time */
    evt->txFlowSpec.accessLatency = event_unpack_CsrUint32(&buffer); /* Access_Latency */
    evt->txFlowSpec.flushTimeout = event_unpack_CsrUint32(&buffer); /* Flush_Timeout */
    evt->rxFlowSpec.id = event_unpack_CsrUint8(&buffer); /* id */
    evt->rxFlowSpec.serviceType = event_unpack_CsrUint8(&buffer); /* Service_Type */
    evt->rxFlowSpec.maximumSduSize = event_unpack_CsrUint16(&buffer); /* Maximum_SDU_Size */
    evt->rxFlowSpec.sduInterArrivalTime = event_unpack_CsrUint32(&buffer); /* SDU_Inter_Arrival_Time */
    evt->rxFlowSpec.accessLatency = event_unpack_CsrUint32(&buffer); /* Access_Latency */
    evt->rxFlowSpec.flushTimeout = event_unpack_CsrUint32(&buffer); /* Flush_Timeout */
    trace_HciAcceptLogicalLinkCmd("", context, evt);

    hci_accept_logical_link_cmd(context, evt->physicalLinkHandle, &evt->txFlowSpec, &evt->rxFlowSpec);

    CsrPfree(evt);

}

static void send_hci_disconnect_logical_link_cmd_unpacked(FsmContext *context, CsrUint8* inputbuffer, CsrUint16 size)
{
    CsrUint8* buffer = inputbuffer;
    HciDisconnectLogicalLinkCmd *evt = (HciDisconnectLogicalLinkCmd*)CsrPmalloc(sizeof(HciDisconnectLogicalLinkCmd));

    evt->logicalLinkHandle = event_unpack_CsrUint16(&buffer); /* Logical_Link_Handle */
    trace_HciDisconnectLogicalLinkCmd("", context, evt);

    hci_disconnect_logical_link_cmd(context, evt->logicalLinkHandle);

    CsrPfree(evt);

}

static void send_hci_logical_link_cancel_cmd_unpacked(FsmContext *context, CsrUint8* inputbuffer, CsrUint16 size)
{
    CsrUint8* buffer = inputbuffer;
    HciLogicalLinkCancelCmd *evt = (HciLogicalLinkCancelCmd*)CsrPmalloc(sizeof(HciLogicalLinkCancelCmd));

    evt->physicalLinkHandle = event_unpack_CsrUint8(&buffer); /* Physical_Link_Handle */
    evt->txFlowSpecId = event_unpack_CsrUint8(&buffer); /* Tx_Flow_Spec_ID */
    trace_HciLogicalLinkCancelCmd("", context, evt);

    hci_logical_link_cancel_cmd(context, evt->physicalLinkHandle, evt->txFlowSpecId);

    CsrPfree(evt);

}

static void send_hci_flow_spec_modify_cmd_unpacked(FsmContext *context, CsrUint8* inputbuffer, CsrUint16 size)
{
    CsrUint8* buffer = inputbuffer;
    HciFlowSpecModifyCmd *evt = (HciFlowSpecModifyCmd*)CsrPmalloc(sizeof(HciFlowSpecModifyCmd));

    evt->handle = event_unpack_CsrUint16(&buffer); /* Handle */
    evt->txFlowSpec.id = event_unpack_CsrUint8(&buffer); /* id */
    evt->txFlowSpec.serviceType = event_unpack_CsrUint8(&buffer); /* Service_Type */
    evt->txFlowSpec.maximumSduSize = event_unpack_CsrUint16(&buffer); /* Maximum_SDU_Size */
    evt->txFlowSpec.sduInterArrivalTime = event_unpack_CsrUint32(&buffer); /* SDU_Inter_Arrival_Time */
    evt->txFlowSpec.accessLatency = event_unpack_CsrUint32(&buffer); /* Access_Latency */
    evt->txFlowSpec.flushTimeout = event_unpack_CsrUint32(&buffer); /* Flush_Timeout */
    evt->rxFlowSpec.id = event_unpack_CsrUint8(&buffer); /* id */
    evt->rxFlowSpec.serviceType = event_unpack_CsrUint8(&buffer); /* Service_Type */
    evt->rxFlowSpec.maximumSduSize = event_unpack_CsrUint16(&buffer); /* Maximum_SDU_Size */
    evt->rxFlowSpec.sduInterArrivalTime = event_unpack_CsrUint32(&buffer); /* SDU_Inter_Arrival_Time */
    evt->rxFlowSpec.accessLatency = event_unpack_CsrUint32(&buffer); /* Access_Latency */
    evt->rxFlowSpec.flushTimeout = event_unpack_CsrUint32(&buffer); /* Flush_Timeout */
    trace_HciFlowSpecModifyCmd("", context, evt);

    hci_flow_spec_modify_cmd(context, evt->handle, &evt->txFlowSpec, &evt->rxFlowSpec);

    CsrPfree(evt);

}

static void send_hci_set_event_mask_cmd_unpacked(FsmContext *context, CsrUint8* inputbuffer, CsrUint16 size)
{
    CsrUint8* buffer = inputbuffer;
    HciSetEventMaskCmd *evt = (HciSetEventMaskCmd*)CsrPmalloc(sizeof(HciSetEventMaskCmd));

    event_unpack_buffer(&buffer, (CsrUint8*)evt->eventMask.m, 8); /*lint !e545 !e725    M */
    trace_HciSetEventMaskCmd("", context, evt);

    hci_set_event_mask_cmd(context, &evt->eventMask);

    CsrPfree(evt);

}

static void send_hci_reset_cmd_unpacked(FsmContext *context, CsrUint8* inputbuffer, CsrUint16 size)
{
    trace_HciResetCmd("", context, NULL);

    hci_reset_cmd(context);


}

static void send_hci_flush_cmd_unpacked(FsmContext *context, CsrUint8* inputbuffer, CsrUint16 size)
{
    CsrUint8* buffer = inputbuffer;
    HciFlushCmd *evt = (HciFlushCmd*)CsrPmalloc(sizeof(HciFlushCmd));

    evt->handle = event_unpack_CsrUint16(&buffer); /* Handle */
    trace_HciFlushCmd("", context, evt);

    hci_flush_cmd(context, evt->handle);

    CsrPfree(evt);

}

static void send_hci_read_connection_accept_timeout_cmd_unpacked(FsmContext *context, CsrUint8* inputbuffer, CsrUint16 size)
{
    trace_HciReadConnectionAcceptTimeoutCmd("", context, NULL);

    hci_read_connection_accept_timeout_cmd(context);


}

static void send_hci_write_connection_accept_timeout_cmd_unpacked(FsmContext *context, CsrUint8* inputbuffer, CsrUint16 size)
{
    CsrUint8* buffer = inputbuffer;
    HciWriteConnectionAcceptTimeoutCmd *evt = (HciWriteConnectionAcceptTimeoutCmd*)CsrPmalloc(sizeof(HciWriteConnectionAcceptTimeoutCmd));

    evt->connAcceptTimeout = event_unpack_CsrUint16(&buffer); /* Conn_Accept_Timeout */
    trace_HciWriteConnectionAcceptTimeoutCmd("", context, evt);

    hci_write_connection_accept_timeout_cmd(context, evt->connAcceptTimeout);

    CsrPfree(evt);

}

static void send_hci_read_link_supervision_timeout_cmd_unpacked(FsmContext *context, CsrUint8* inputbuffer, CsrUint16 size)
{
    CsrUint8* buffer = inputbuffer;
    HciReadLinkSupervisionTimeoutCmd *evt = (HciReadLinkSupervisionTimeoutCmd*)CsrPmalloc(sizeof(HciReadLinkSupervisionTimeoutCmd));

    evt->handle = event_unpack_CsrUint16(&buffer); /* Handle */
    trace_HciReadLinkSupervisionTimeoutCmd("", context, evt);

    hci_read_link_supervision_timeout_cmd(context, evt->handle);

    CsrPfree(evt);

}

static void send_hci_write_link_supervision_timeout_cmd_unpacked(FsmContext *context, CsrUint8* inputbuffer, CsrUint16 size)
{
    CsrUint8* buffer = inputbuffer;
    HciWriteLinkSupervisionTimeoutCmd *evt = (HciWriteLinkSupervisionTimeoutCmd*)CsrPmalloc(sizeof(HciWriteLinkSupervisionTimeoutCmd));

    evt->handle = event_unpack_CsrUint16(&buffer); /* Handle */
    evt->linkSupervisionTimeout = event_unpack_CsrUint16(&buffer); /* Link_Supervision_Timeout */
    trace_HciWriteLinkSupervisionTimeoutCmd("", context, evt);

    hci_write_link_supervision_timeout_cmd(context, evt->handle, evt->linkSupervisionTimeout);

    CsrPfree(evt);

}

static void send_hci_enhanced_flush_cmd_unpacked(FsmContext *context, CsrUint8* inputbuffer, CsrUint16 size)
{
    CsrUint8* buffer = inputbuffer;
    HciEnhancedFlushCmd *evt = (HciEnhancedFlushCmd*)CsrPmalloc(sizeof(HciEnhancedFlushCmd));

    evt->handle = event_unpack_CsrUint16(&buffer); /* Handle */
    evt->packetType = event_unpack_CsrUint8(&buffer); /* Packet_Type */
    trace_HciEnhancedFlushCmd("", context, evt);

    hci_enhanced_flush_cmd(context, evt->handle, evt->packetType);

    CsrPfree(evt);

}

static void send_hci_read_logical_link_accept_timeout_cmd_unpacked(FsmContext *context, CsrUint8* inputbuffer, CsrUint16 size)
{
    trace_HciReadLogicalLinkAcceptTimeoutCmd("", context, NULL);

    hci_read_logical_link_accept_timeout_cmd(context);


}

static void send_hci_write_logical_link_accept_timeout_cmd_unpacked(FsmContext *context, CsrUint8* inputbuffer, CsrUint16 size)
{
    CsrUint8* buffer = inputbuffer;
    HciWriteLogicalLinkAcceptTimeoutCmd *evt = (HciWriteLogicalLinkAcceptTimeoutCmd*)CsrPmalloc(sizeof(HciWriteLogicalLinkAcceptTimeoutCmd));

    evt->logicalLinkAcceptTimeout = event_unpack_CsrUint16(&buffer); /* Logical_Link_Accept_Timeout */
    trace_HciWriteLogicalLinkAcceptTimeoutCmd("", context, evt);

    hci_write_logical_link_accept_timeout_cmd(context, evt->logicalLinkAcceptTimeout);

    CsrPfree(evt);

}

static void send_hci_set_event_mask_page2_cmd_unpacked(FsmContext *context, CsrUint8* inputbuffer, CsrUint16 size)
{
    CsrUint8* buffer = inputbuffer;
    HciSetEventMaskPage2Cmd *evt = (HciSetEventMaskPage2Cmd*)CsrPmalloc(sizeof(HciSetEventMaskPage2Cmd));

    event_unpack_buffer(&buffer, (CsrUint8*)evt->eventMaskPage2.m, 8); /*lint !e545 !e725    M */
    trace_HciSetEventMaskPage2Cmd("", context, evt);

    hci_set_event_mask_page2_cmd(context, &evt->eventMaskPage2);

    CsrPfree(evt);

}

static void send_hci_read_location_data_cmd_unpacked(FsmContext *context, CsrUint8* inputbuffer, CsrUint16 size)
{
    trace_HciReadLocationDataCmd("", context, NULL);

    hci_read_location_data_cmd(context);


}

static void send_hci_write_location_data_cmd_unpacked(FsmContext *context, CsrUint8* inputbuffer, CsrUint16 size)
{
    CsrUint8* buffer = inputbuffer;
    HciWriteLocationDataCmd *evt = (HciWriteLocationDataCmd*)CsrPmalloc(sizeof(HciWriteLocationDataCmd));

    evt->locationDomainAware = event_unpack_CsrUint8(&buffer); /* Location_Domain_Aware */
    evt->locationDomain = event_unpack_CsrUint16(&buffer); /* Location_Domain */
    evt->locationDomainOptions = event_unpack_CsrUint8(&buffer); /* Location_Domain_Options */
    evt->locationOptions = event_unpack_CsrUint8(&buffer); /* Location_Options */
    trace_HciWriteLocationDataCmd("", context, evt);

    hci_write_location_data_cmd(context, evt->locationDomainAware, evt->locationDomain, evt->locationDomainOptions, evt->locationOptions);

    CsrPfree(evt);

}

static void send_hci_read_best_effort_flush_timeout_cmd_unpacked(FsmContext *context, CsrUint8* inputbuffer, CsrUint16 size)
{
    CsrUint8* buffer = inputbuffer;
    HciReadBestEffortFlushTimeoutCmd *evt = (HciReadBestEffortFlushTimeoutCmd*)CsrPmalloc(sizeof(HciReadBestEffortFlushTimeoutCmd));

    evt->logicalLinkHandle = event_unpack_CsrUint16(&buffer); /* Logical_Link_Handle */
    trace_HciReadBestEffortFlushTimeoutCmd("", context, evt);

    hci_read_best_effort_flush_timeout_cmd(context, evt->logicalLinkHandle);

    CsrPfree(evt);

}

static void send_hci_write_best_effort_flush_timeout_cmd_unpacked(FsmContext *context, CsrUint8* inputbuffer, CsrUint16 size)
{
    CsrUint8* buffer = inputbuffer;
    HciWriteBestEffortFlushTimeoutCmd *evt = (HciWriteBestEffortFlushTimeoutCmd*)CsrPmalloc(sizeof(HciWriteBestEffortFlushTimeoutCmd));

    evt->logicalLinkHandle = event_unpack_CsrUint16(&buffer); /* Logical_Link_Handle */
    evt->bestEffortFlushTimeout = event_unpack_CsrUint32(&buffer); /* Best_Effort_Flush_Timeout */
    trace_HciWriteBestEffortFlushTimeoutCmd("", context, evt);

    hci_write_best_effort_flush_timeout_cmd(context, evt->logicalLinkHandle, evt->bestEffortFlushTimeout);

    CsrPfree(evt);

}

static void send_hci_short_range_mode_cmd_unpacked(FsmContext *context, CsrUint8* inputbuffer, CsrUint16 size)
{
    CsrUint8* buffer = inputbuffer;
    HciShortRangeModeCmd *evt = (HciShortRangeModeCmd*)CsrPmalloc(sizeof(HciShortRangeModeCmd));

    evt->physicalLinkHandle = event_unpack_CsrUint8(&buffer); /* Physical_Link_Handle */
    evt->shortRangeMode = event_unpack_CsrUint8(&buffer); /* Short_Range_Mode */
    trace_HciShortRangeModeCmd("", context, evt);

    hci_short_range_mode_cmd(context, evt->physicalLinkHandle, evt->shortRangeMode);

    CsrPfree(evt);

}

static void send_hci_read_local_version_information_cmd_unpacked(FsmContext *context, CsrUint8* inputbuffer, CsrUint16 size)
{
    trace_HciReadLocalVersionInformationCmd("", context, NULL);

    hci_read_local_version_information_cmd(context);


}

static void send_hci_read_local_supported_commands_cmd_unpacked(FsmContext *context, CsrUint8* inputbuffer, CsrUint16 size)
{
    trace_HciReadLocalSupportedCommandsCmd("", context, NULL);

    hci_read_local_supported_commands_cmd(context);


}

static void send_hci_read_data_block_size_cmd_unpacked(FsmContext *context, CsrUint8* inputbuffer, CsrUint16 size)
{
    trace_HciReadDataBlockSizeCmd("", context, NULL);

    hci_read_data_block_size_cmd(context);


}

static void send_hci_read_failed_contact_counter_cmd_unpacked(FsmContext *context, CsrUint8* inputbuffer, CsrUint16 size)
{
    CsrUint8* buffer = inputbuffer;
    HciReadFailedContactCounterCmd *evt = (HciReadFailedContactCounterCmd*)CsrPmalloc(sizeof(HciReadFailedContactCounterCmd));

    evt->handle = event_unpack_CsrUint16(&buffer); /* Handle */
    trace_HciReadFailedContactCounterCmd("", context, evt);

    hci_read_failed_contact_counter_cmd(context, evt->handle);

    CsrPfree(evt);

}

static void send_hci_reset_failed_contact_counter_cmd_unpacked(FsmContext *context, CsrUint8* inputbuffer, CsrUint16 size)
{
    CsrUint8* buffer = inputbuffer;
    HciResetFailedContactCounterCmd *evt = (HciResetFailedContactCounterCmd*)CsrPmalloc(sizeof(HciResetFailedContactCounterCmd));

    evt->handle = event_unpack_CsrUint16(&buffer); /* Handle */
    trace_HciResetFailedContactCounterCmd("", context, evt);

    hci_reset_failed_contact_counter_cmd(context, evt->handle);

    CsrPfree(evt);

}

static void send_hci_read_link_quality_cmd_unpacked(FsmContext *context, CsrUint8* inputbuffer, CsrUint16 size)
{
    CsrUint8* buffer = inputbuffer;
    HciReadLinkQualityCmd *evt = (HciReadLinkQualityCmd*)CsrPmalloc(sizeof(HciReadLinkQualityCmd));

    evt->handle = event_unpack_CsrUint16(&buffer); /* Handle */
    trace_HciReadLinkQualityCmd("", context, evt);

    hci_read_link_quality_cmd(context, evt->handle);

    CsrPfree(evt);

}

static void send_hci_read_rssi_cmd_unpacked(FsmContext *context, CsrUint8* inputbuffer, CsrUint16 size)
{
    CsrUint8* buffer = inputbuffer;
    HciReadRssiCmd *evt = (HciReadRssiCmd*)CsrPmalloc(sizeof(HciReadRssiCmd));

    evt->handle = event_unpack_CsrUint16(&buffer); /* Handle */
    trace_HciReadRssiCmd("", context, evt);

    hci_read_rssi_cmd(context, evt->handle);

    CsrPfree(evt);

}

static void send_hci_read_local_amp_info_cmd_unpacked(FsmContext *context, CsrUint8* inputbuffer, CsrUint16 size)
{
    trace_HciReadLocalAmpInfoCmd("", context, NULL);

    hci_read_local_amp_info_cmd(context);


}

static void send_hci_read_local_amp_assoc_cmd_unpacked(FsmContext *context, CsrUint8* inputbuffer, CsrUint16 size)
{
    CsrUint8* buffer = inputbuffer;
    HciReadLocalAmpAssocCmd *evt = (HciReadLocalAmpAssocCmd*)CsrPmalloc(sizeof(HciReadLocalAmpAssocCmd));

    evt->physicalLinkHandle = event_unpack_CsrUint8(&buffer); /* Physical_Link_Handle */
    evt->lengthSoFar = event_unpack_CsrUint16(&buffer); /* Length_So_Far */
    evt->maxRemoteAmpAssocLength = event_unpack_CsrUint16(&buffer); /* Max_Remote_AMP_ASSOC_Length */
    trace_HciReadLocalAmpAssocCmd("", context, evt);

    hci_read_local_amp_assoc_cmd(context, evt->physicalLinkHandle, evt->lengthSoFar, evt->maxRemoteAmpAssocLength);

    CsrPfree(evt);

}

static void send_hci_write_remote_amp_assoc_cmd_unpacked(FsmContext *context, CsrUint8* inputbuffer, CsrUint16 size)
{
    CsrUint8* buffer = inputbuffer;
    HciWriteRemoteAmpAssocCmd *evt;
    CsrUint8 dataLen;
    buffer = (inputbuffer+5); /* Move to the dataLen */
    dataLen = (CsrUint8)(size-5); /* dataLen */
    buffer = inputbuffer; /* Restore for normal decode */
    evt = (HciWriteRemoteAmpAssocCmd*)CsrPmalloc(sizeof(HciWriteRemoteAmpAssocCmd)+dataLen);

    evt->physicalLinkHandle = event_unpack_CsrUint8(&buffer); /* Physical_Link_Handle */
    evt->lengthSoFar = event_unpack_CsrUint16(&buffer); /* Length_So_Far */
    evt->remainingLength = event_unpack_CsrUint16(&buffer); /* Remaining_Length */
    evt->assocFragment.dataLen= dataLen; /* save the length of the var length data */
    evt->assocFragment.data = (CsrUint8 *)(evt+1); /* Point to bottom of the structure to store var len data */
    event_unpack_buffer(&buffer, (CsrUint8*)evt->assocFragment.data, 1 * evt->assocFragment.dataLen); /*lint !e545 !e725    Data */
    trace_HciWriteRemoteAmpAssocCmd("", context, evt);

    hci_write_remote_amp_assoc_cmd(context, evt->physicalLinkHandle, evt->lengthSoFar, evt->remainingLength, evt->assocFragment);

    CsrPfree(evt);

}

static void send_hci_read_loopback_mode_cmd_unpacked(FsmContext *context, CsrUint8* inputbuffer, CsrUint16 size)
{
    trace_HciReadLoopbackModeCmd("", context, NULL);

    hci_read_loopback_mode_cmd(context);


}

static void send_hci_write_loopback_mode_cmd_unpacked(FsmContext *context, CsrUint8* inputbuffer, CsrUint16 size)
{
    CsrUint8* buffer = inputbuffer;
    HciWriteLoopbackModeCmd *evt = (HciWriteLoopbackModeCmd*)CsrPmalloc(sizeof(HciWriteLoopbackModeCmd));

    evt->loopbackMode = event_unpack_CsrUint8(&buffer); /* Loopback_Mode */
    trace_HciWriteLoopbackModeCmd("", context, evt);

    hci_write_loopback_mode_cmd(context, evt->loopbackMode);

    CsrPfree(evt);

}

static void send_hci_enable_amp_test_mode_cmd_unpacked(FsmContext *context, CsrUint8* inputbuffer, CsrUint16 size)
{
    CsrUint8* buffer = inputbuffer;
    HciEnableAmpTestModeCmd *evt = (HciEnableAmpTestModeCmd*)CsrPmalloc(sizeof(HciEnableAmpTestModeCmd));

    evt->enable = event_unpack_CsrUint8(&buffer); /* Enable */
    trace_HciEnableAmpTestModeCmd("", context, evt);

    hci_enable_amp_test_mode_cmd(context, evt->enable);

    CsrPfree(evt);

}

static void send_hci_set_amp_transmit_power_test_cmd_unpacked(FsmContext *context, CsrUint8* inputbuffer, CsrUint16 size)
{
    CsrUint8* buffer = inputbuffer;
    HciSetAmpTransmitPowerTestCmd *evt = (HciSetAmpTransmitPowerTestCmd*)CsrPmalloc(sizeof(HciSetAmpTransmitPowerTestCmd));

    evt->powerLevel = event_unpack_CsrUint8(&buffer); /* Power_Level */
    trace_HciSetAmpTransmitPowerTestCmd("", context, evt);

    hci_set_amp_transmit_power_test_cmd(context, evt->powerLevel);

    CsrPfree(evt);

}

static void send_hci_enable_amp_receiver_reports_cmd_unpacked(FsmContext *context, CsrUint8* inputbuffer, CsrUint16 size)
{
    CsrUint8* buffer = inputbuffer;
    HciEnableAmpReceiverReportsCmd *evt = (HciEnableAmpReceiverReportsCmd*)CsrPmalloc(sizeof(HciEnableAmpReceiverReportsCmd));

    evt->enable = event_unpack_CsrUint8(&buffer); /* Enable */
    evt->interval = event_unpack_CsrUint8(&buffer); /* Interval */
    trace_HciEnableAmpReceiverReportsCmd("", context, evt);

    hci_enable_amp_receiver_reports_cmd(context, evt->enable, evt->interval);

    CsrPfree(evt);

}

static void send_hci_transmitter_amp_test_cmd_unpacked(FsmContext *context, CsrUint8* inputbuffer, CsrUint16 size)
{
    CsrUint8* buffer = inputbuffer;
    HciTransmitterAmpTestCmd *evt = (HciTransmitterAmpTestCmd*)CsrPmalloc(sizeof(HciTransmitterAmpTestCmd));

    evt->testScenario = event_unpack_CsrUint8(&buffer); /* Test_Scenario */
    trace_HciTransmitterAmpTestCmd("", context, evt);

    hci_transmitter_amp_test_cmd(context, evt->testScenario);

    CsrPfree(evt);

}

static void send_hci_receiver_amp_test_cmd_unpacked(FsmContext *context, CsrUint8* inputbuffer, CsrUint16 size)
{
    CsrUint8* buffer = inputbuffer;
    HciReceiverAmpTestCmd *evt = (HciReceiverAmpTestCmd*)CsrPmalloc(sizeof(HciReceiverAmpTestCmd));

    evt->testScenario = event_unpack_CsrUint8(&buffer); /* Test_Scenario */
    trace_HciReceiverAmpTestCmd("", context, evt);

    hci_receiver_amp_test_cmd(context, evt->testScenario);

    CsrPfree(evt);

}

static void send_hci_amp_test_end_cmd_unpacked(FsmContext *context, CsrUint8* inputbuffer, CsrUint16 size)
{
    trace_HciAmpTestEndCmd("", context, NULL);

    hci_amp_test_end_cmd(context);


}

typedef struct HciCommandHeader 
{
    HciOpcode opcode;
    CsrUint8 length;
} HciCommandHeader;

CsrBool unpack_pal_hci_signalDown_received(FsmContext *context, CsrUint8* inbuffer, CsrUint16 size)
{
    CsrUint8* buffer = inbuffer;

    /* decode generic header */
    CsrUint16 newSize;
    HciCommandHeader genHdr;
    genHdr.opcode = event_unpack_CsrUint16(&buffer); /* Opcode */
    genHdr.length = event_unpack_CsrUint8(&buffer); /* Length */
    newSize = size - (CsrUint16)(buffer-inbuffer);

    switch(genHdr.opcode) {
        case HCI_NO_OPERATION_CODE:
            send_hci_no_operation_cmd_unpacked(context, buffer, newSize);
            return TRUE;

        case HCI_CREATE_PHYSICAL_LINK_CODE:
            send_hci_create_physical_link_cmd_unpacked(context, buffer, newSize);
            return TRUE;

        case HCI_ACCEPT_PHYSICAL_LINK_REQUEST_CODE:
            send_hci_accept_physical_link_request_cmd_unpacked(context, buffer, newSize);
            return TRUE;

        case HCI_DISCONNECT_PHYSICAL_LINK_CODE:
            send_hci_disconnect_physical_link_cmd_unpacked(context, buffer, newSize);
            return TRUE;

        case HCI_CREATE_LOGICAL_LINK_CODE:
            send_hci_create_logical_link_cmd_unpacked(context, buffer, newSize);
            return TRUE;

        case HCI_ACCEPT_LOGICAL_LINK_CODE:
            send_hci_accept_logical_link_cmd_unpacked(context, buffer, newSize);
            return TRUE;

        case HCI_DISCONNECT_LOGICAL_LINK_CODE:
            send_hci_disconnect_logical_link_cmd_unpacked(context, buffer, newSize);
            return TRUE;

        case HCI_LOGICAL_LINK_CANCEL_CODE:
            send_hci_logical_link_cancel_cmd_unpacked(context, buffer, newSize);
            return TRUE;

        case HCI_FLOW_SPEC_MODIFY_CODE:
            send_hci_flow_spec_modify_cmd_unpacked(context, buffer, newSize);
            return TRUE;

        case HCI_SET_EVENT_MASK_CODE:
            send_hci_set_event_mask_cmd_unpacked(context, buffer, newSize);
            return TRUE;

        case HCI_RESET_CODE:
            send_hci_reset_cmd_unpacked(context, buffer, newSize);
            return TRUE;

        case HCI_FLUSH_CODE:
            send_hci_flush_cmd_unpacked(context, buffer, newSize);
            return TRUE;

        case HCI_READ_CONNECTION_ACCEPT_TIMEOUT_CODE:
            send_hci_read_connection_accept_timeout_cmd_unpacked(context, buffer, newSize);
            return TRUE;

        case HCI_WRITE_CONNECTION_ACCEPT_TIMEOUT_CODE:
            send_hci_write_connection_accept_timeout_cmd_unpacked(context, buffer, newSize);
            return TRUE;

        case HCI_READ_LINK_SUPERVISION_TIMEOUT_CODE:
            send_hci_read_link_supervision_timeout_cmd_unpacked(context, buffer, newSize);
            return TRUE;

        case HCI_WRITE_LINK_SUPERVISION_TIMEOUT_CODE:
            send_hci_write_link_supervision_timeout_cmd_unpacked(context, buffer, newSize);
            return TRUE;

        case HCI_ENHANCED_FLUSH_CODE:
            send_hci_enhanced_flush_cmd_unpacked(context, buffer, newSize);
            return TRUE;

        case HCI_READ_LOGICAL_LINK_ACCEPT_TIMEOUT_CODE:
            send_hci_read_logical_link_accept_timeout_cmd_unpacked(context, buffer, newSize);
            return TRUE;

        case HCI_WRITE_LOGICAL_LINK_ACCEPT_TIMEOUT_CODE:
            send_hci_write_logical_link_accept_timeout_cmd_unpacked(context, buffer, newSize);
            return TRUE;

        case HCI_SET_EVENT_MASK_PAGE2_CODE:
            send_hci_set_event_mask_page2_cmd_unpacked(context, buffer, newSize);
            return TRUE;

        case HCI_READ_LOCATION_DATA_CODE:
            send_hci_read_location_data_cmd_unpacked(context, buffer, newSize);
            return TRUE;

        case HCI_WRITE_LOCATION_DATA_CODE:
            send_hci_write_location_data_cmd_unpacked(context, buffer, newSize);
            return TRUE;

        case HCI_READ_BEST_EFFORT_FLUSH_TIMEOUT_CODE:
            send_hci_read_best_effort_flush_timeout_cmd_unpacked(context, buffer, newSize);
            return TRUE;

        case HCI_WRITE_BEST_EFFORT_FLUSH_TIMEOUT_CODE:
            send_hci_write_best_effort_flush_timeout_cmd_unpacked(context, buffer, newSize);
            return TRUE;

        case HCI_SHORT_RANGE_MODE_CODE:
            send_hci_short_range_mode_cmd_unpacked(context, buffer, newSize);
            return TRUE;

        case HCI_READ_LOCAL_VERSION_INFORMATION_CODE:
            send_hci_read_local_version_information_cmd_unpacked(context, buffer, newSize);
            return TRUE;

        case HCI_READ_LOCAL_SUPPORTED_COMMANDS_CODE:
            send_hci_read_local_supported_commands_cmd_unpacked(context, buffer, newSize);
            return TRUE;

        case HCI_READ_DATA_BLOCK_SIZE_CODE:
            send_hci_read_data_block_size_cmd_unpacked(context, buffer, newSize);
            return TRUE;

        case HCI_READ_FAILED_CONTACT_COUNTER_CODE:
            send_hci_read_failed_contact_counter_cmd_unpacked(context, buffer, newSize);
            return TRUE;

        case HCI_RESET_FAILED_CONTACT_COUNTER_CODE:
            send_hci_reset_failed_contact_counter_cmd_unpacked(context, buffer, newSize);
            return TRUE;

        case HCI_READ_LINK_QUALITY_CODE:
            send_hci_read_link_quality_cmd_unpacked(context, buffer, newSize);
            return TRUE;

        case HCI_READ_RSSI_CODE:
            send_hci_read_rssi_cmd_unpacked(context, buffer, newSize);
            return TRUE;

        case HCI_READ_LOCAL_AMP_INFO_CODE:
            send_hci_read_local_amp_info_cmd_unpacked(context, buffer, newSize);
            return TRUE;

        case HCI_READ_LOCAL_AMP_ASSOC_CODE:
            send_hci_read_local_amp_assoc_cmd_unpacked(context, buffer, newSize);
            return TRUE;

        case HCI_WRITE_REMOTE_AMP_ASSOC_CODE:
            send_hci_write_remote_amp_assoc_cmd_unpacked(context, buffer, newSize);
            return TRUE;

        case HCI_READ_LOOPBACK_MODE_CODE:
            send_hci_read_loopback_mode_cmd_unpacked(context, buffer, newSize);
            return TRUE;

        case HCI_WRITE_LOOPBACK_MODE_CODE:
            send_hci_write_loopback_mode_cmd_unpacked(context, buffer, newSize);
            return TRUE;

        case HCI_ENABLE_AMP_TEST_MODE_CODE:
            send_hci_enable_amp_test_mode_cmd_unpacked(context, buffer, newSize);
            return TRUE;

        case HCI_SET_AMP_TRANSMIT_POWER_TEST_CODE:
            send_hci_set_amp_transmit_power_test_cmd_unpacked(context, buffer, newSize);
            return TRUE;

        case HCI_ENABLE_AMP_RECEIVER_REPORTS_CODE:
            send_hci_enable_amp_receiver_reports_cmd_unpacked(context, buffer, newSize);
            return TRUE;

        case HCI_TRANSMITTER_AMP_TEST_CODE:
            send_hci_transmitter_amp_test_cmd_unpacked(context, buffer, newSize);
            return TRUE;

        case HCI_RECEIVER_AMP_TEST_CODE:
            send_hci_receiver_amp_test_cmd_unpacked(context, buffer, newSize);
            return TRUE;

        case HCI_AMP_TEST_END_CODE:
            send_hci_amp_test_end_cmd_unpacked(context, buffer, newSize);
            return TRUE;

        default:
            break;
    }
    return FALSE;
}

