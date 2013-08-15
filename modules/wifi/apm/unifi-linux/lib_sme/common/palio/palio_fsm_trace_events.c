/* This is an autogenerated file */
/* Tag: noCheckHeader */

/*    CONFIDENTIAL */
/*    Copyright (C) Cambridge Silicon Radio Ltd 2008. All rights reserved. */


#include "fsm/fsm_internal.h"

#ifdef SME_TRACE_ENABLE
#ifndef SME_TRACE_NO_MSC

#include "palio/palio_trace_signals.h"


typedef void (*traceFn)(const char* text, const FsmContext* context, const FsmEvent* event);

static const traceFn fnlookup1[29] = {
    (traceFn)trace_PalCtrlLinkCreateReq,
    (traceFn)trace_PalCtrlLinkCreateCfm,
    (traceFn)trace_PalCtrlLinkDeleteReq,
    (traceFn)trace_PalCtrlLinkDeleteCfm,
    (traceFn)trace_PalCtrlLinkModifyReq,
    (traceFn)trace_PalCtrlLinkModifyCfm,
    (traceFn)trace_PalCtrlEventMaskSetReq,
    (traceFn)trace_PalCtrlEventMaskSetCfm,
    (traceFn)trace_PalCtrlLinkFlushReq,
    (traceFn)trace_PalCtrlLinkFlushCfm,
    (traceFn)trace_PalCtrlFailedContactCounterReadReq,
    (traceFn)trace_PalCtrlFailedContactCounterReadCfm,
    (traceFn)trace_PalCtrlFailedContactCounterResetReq,
    (traceFn)trace_PalCtrlFailedContactCounterResetCfm,
    (traceFn)trace_PalCtrlActivateReq,
    (traceFn)trace_PalCtrlActivateCfm,
    (traceFn)trace_PalCtrlDeactivateReq,
    (traceFn)trace_PalCtrlDeactivateCfm,
    (traceFn)trace_PalCtrlLinkSupervisionTimeoutSetReq,
    (traceFn)trace_PalCtrlLinkSupervisionTimeoutSetCfm,
    (traceFn)trace_PalCtrlLinkSupervisionTimeoutModifyReq,
    (traceFn)trace_PalCtrlLinkSupervisionTimeoutModifyCfm,
    (traceFn)trace_PalCtrlLinkSupervisionTimeoutDeleteReq,
    (traceFn)trace_PalCtrlLinkSupervisionTimeoutDeleteCfm,
    (traceFn)trace_PalCtrlEarlyLinkLossInd,
    (traceFn)trace_PalCtrlLinkLostInd,
    (traceFn)trace_PalCtrlLinkAliveReq,
    (traceFn)trace_PalCtrlRegisterReq,
    (traceFn)trace_PalCtrlRegisterCfm,
};
static const traceFn fnlookup2[15] = {
    (traceFn)trace_PalLinkConnectReq,
    (traceFn)trace_PalLinkConnectCfm,
    (traceFn)trace_PalLinkAuthenticateReq,
    (traceFn)trace_PalLinkAuthenticateCfm,
    (traceFn)trace_PalLinkDisconnectReq,
    (traceFn)trace_PalLinkDisconnectCfm,
    (traceFn)trace_PalLinkDisconnectInd,
    (traceFn)trace_PalLinkStartReq,
    (traceFn)trace_PalLinkStartCfm,
    (traceFn)trace_PalLinkStopReq,
    (traceFn)trace_PalLinkStopCfm,
    (traceFn)trace_PalLinkLinkSupervisionRequestReq,
    (traceFn)trace_PalLinkLinkSupervisionResponseInd,
    (traceFn)trace_PalLinkTimer,
    (traceFn)trace_PalSecurityHandshakeTimer,
};
static const traceFn fnlookup3[10] = {
    (traceFn)trace_PalDmStartReq,
    (traceFn)trace_PalDmStartCfm,
    (traceFn)trace_PalDmMlmeActivityStartReq,
    (traceFn)trace_PalDmMlmeActivityStartCfm,
    (traceFn)trace_PalDmMlmeActivityCompleteReq,
    (traceFn)trace_PalDmMlmeActivityCompleteCfm,
    (traceFn)trace_PalDmStopReq,
    (traceFn)trace_PalDmStopCfm,
    (traceFn)trace_PalDmEnableRtsCtsReq,
    (traceFn)trace_PalDmDisableRtsCtsReq,
};
static const traceFn fnlookup4[7] = {
    (traceFn)trace_PalCoexLinkConnectingReq,
    (traceFn)trace_PalCoexLinkConnectedReq,
    (traceFn)trace_PalCoexLinkConnectedCfm,
    (traceFn)trace_PalCoexLinkDisconnectedReq,
    (traceFn)trace_PalCoexLinkDisconnectedCfm,
    (traceFn)trace_PalCoexLocalActivityReportInd,
    (traceFn)trace_PalCoexRemoteActivityReportReq,
};


CsrBool trace_palio_fsm_event(const char* text, const FsmContext* context, const FsmEvent *event)
{
    CsrUint16 id = event->id;
    if (id >= 0x2000 && id <= 0x201c){ (*fnlookup1[id - 0x2000])("", context, event); return TRUE;}
    if (id >= 0x2300 && id <= 0x230e){ (*fnlookup2[id - 0x2300])("", context, event); return TRUE;}
    if (id >= 0x2201 && id <= 0x220a){ (*fnlookup3[id - 0x2201])("", context, event); return TRUE;}
    if (id >= 0x2401 && id <= 0x2407){ (*fnlookup4[id - 0x2401])("", context, event); return TRUE;}


    return FALSE;
}

#endif /* SME_TRACE_NO_MSC */
#endif /* SME_TRACE_ENABLE */