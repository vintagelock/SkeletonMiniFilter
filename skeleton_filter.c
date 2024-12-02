/*++

Skeleton MiniFilter Driver

--*/

#include <fltKernel.h>
#include <dontuse.h>
#include <suppress.h>

#pragma prefast(disable:__WARNING_ENCODE_MEMBER_FUNCTION_POINTER, "Not valid for kernel mode drivers")

#include "skeleton_filter.h"
#include "context.h"

#include "create.h"
#include "cleanup.h"
#include "close.h"
#include "read.h"
#include "write.h"

// Globals
PFLT_FILTER gFilterHandle;
ULONG_PTR   OperationStatusCtx = 1;

//////////////////////////////////////////////////////////////////////////



//
//  Assign text sections for each routine.
//
#ifdef ALLOC_PRAGMA
    #pragma alloc_text(INIT, DriverEntry)
    #pragma alloc_text(PAGE, SKEL_Unload)
    #pragma alloc_text(PAGE, SKEL_InstanceQueryTeardown)
    #pragma alloc_text(PAGE, SKEL_InstanceSetup)
    #pragma alloc_text(PAGE, SKEL_InstanceTeardownStart)
    #pragma alloc_text(PAGE, SKEL_InstanceTeardownComplete)
#endif

//
//  operation registration
//
CONST FLT_OPERATION_REGISTRATION Callbacks[] = {
    { 
        IRP_MJ_CREATE,
        0,
        SKEL_PreCreate,
		SKEL_PostCreate
    },
    { 
        IRP_MJ_CREATE_NAMED_PIPE,
        0,
		SKEL_PreOperationPassThrough,
		SKEL_PostOperationPassThrough
    },
    { 
        IRP_MJ_CLOSE,
        0,
        SKEL_PreClose,
        SKEL_PostClose
    },
    { 
        IRP_MJ_READ,
        0,
        SKEL_PreRead,
        SKEL_PostRead
    },
    { 
        IRP_MJ_WRITE,
        0,
        SKEL_PreWrite,
        SKEL_PostWrite
    },
    { 
        IRP_MJ_QUERY_INFORMATION,
        0,
        SKEL_PreOperationPassThrough,
        SKEL_PostOperationPassThrough 
    },
    { 
        IRP_MJ_SET_INFORMATION,
        0,
        SKEL_PreOperationPassThrough,
        SKEL_PostOperationPassThrough 
    },
    { 
        IRP_MJ_QUERY_EA,
        0,
        SKEL_PreOperationPassThrough,
        SKEL_PostOperationPassThrough 
    },
    { 
        IRP_MJ_SET_EA,
        0,
        SKEL_PreOperationPassThrough,
        SKEL_PostOperationPassThrough 
    },
    { 
        IRP_MJ_FLUSH_BUFFERS,
        0,
        SKEL_PreOperationPassThrough,
        SKEL_PostOperationPassThrough 
    },
    { 
        IRP_MJ_QUERY_VOLUME_INFORMATION,
        0,
        SKEL_PreOperationPassThrough,
        SKEL_PostOperationPassThrough 
    },
    { 
        IRP_MJ_SET_VOLUME_INFORMATION,
        0,
        SKEL_PreOperationPassThrough,
        SKEL_PostOperationPassThrough 
    },
    { 
        IRP_MJ_DIRECTORY_CONTROL,
        0,
        SKEL_PreOperationPassThrough,
        SKEL_PostOperationPassThrough 
    },
    { 
        IRP_MJ_FILE_SYSTEM_CONTROL,
        0,
        SKEL_PreOperationPassThrough,
        SKEL_PostOperationPassThrough 
    },
    { 
        IRP_MJ_DEVICE_CONTROL,
        0,
        SKEL_PreOperationPassThrough,
        SKEL_PostOperationPassThrough 
    },
    { 
        IRP_MJ_INTERNAL_DEVICE_CONTROL,
        0,
        SKEL_PreOperationPassThrough,
        SKEL_PostOperationPassThrough 
    },
    { 
        IRP_MJ_SHUTDOWN,
        0,
        SKEL_PreOperationNoPostOperationPassThrough,
        NULL                                // No post
    },
    { 
        IRP_MJ_LOCK_CONTROL,
        0,
        SKEL_PreOperationPassThrough,
        SKEL_PostOperationPassThrough 
    },
    { 
        IRP_MJ_CLEANUP,
        0,
        SKEL_PreCleanup,
        SKEL_PostCleanup
    },
    { 
        IRP_MJ_CREATE_MAILSLOT,
        0,
        SKEL_PreOperationPassThrough,
        SKEL_PostOperationPassThrough 
    },
    { 
        IRP_MJ_QUERY_SECURITY,
        0,
        SKEL_PreOperationPassThrough,
        SKEL_PostOperationPassThrough 
    },
    { 
        IRP_MJ_SET_SECURITY,
        0,
        SKEL_PreOperationPassThrough,
        SKEL_PostOperationPassThrough 
    },
    { 
        IRP_MJ_QUERY_QUOTA,
        0,
        SKEL_PreOperationPassThrough,
        SKEL_PostOperationPassThrough 
    },
    { 
        IRP_MJ_SET_QUOTA,
        0,
        SKEL_PreOperationPassThrough,
        SKEL_PostOperationPassThrough 
    },
    { 
        IRP_MJ_PNP,
        0,
        SKEL_PreOperationPassThrough,
        SKEL_PostOperationPassThrough 
    },
    { 
        IRP_MJ_ACQUIRE_FOR_SECTION_SYNCHRONIZATION,
        0,
        SKEL_PreOperationPassThrough,
        SKEL_PostOperationPassThrough 
    },
    { 
        IRP_MJ_RELEASE_FOR_SECTION_SYNCHRONIZATION,
        0,
        SKEL_PreOperationPassThrough,
        SKEL_PostOperationPassThrough 
    },
    { 
        IRP_MJ_ACQUIRE_FOR_MOD_WRITE,
        0,
        SKEL_PreOperationPassThrough,
        SKEL_PostOperationPassThrough 
    },
    { 
        IRP_MJ_RELEASE_FOR_MOD_WRITE,
        0,
        SKEL_PreOperationPassThrough,
        SKEL_PostOperationPassThrough 
    },
    { 
        IRP_MJ_ACQUIRE_FOR_CC_FLUSH,
        0,
        SKEL_PreOperationPassThrough,
        SKEL_PostOperationPassThrough 
    },
    { 
        IRP_MJ_RELEASE_FOR_CC_FLUSH,
        0,
        SKEL_PreOperationPassThrough,
        SKEL_PostOperationPassThrough 
    },
    { 
        IRP_MJ_FAST_IO_CHECK_IF_POSSIBLE,
        0,
        SKEL_PreOperationPassThrough,
        SKEL_PostOperationPassThrough 
    },
    { 
        IRP_MJ_NETWORK_QUERY_OPEN,
        0,
        SKEL_PreOperationPassThrough,
        SKEL_PostOperationPassThrough 
    },
    { 
        IRP_MJ_MDL_READ,
        0,
        SKEL_PreOperationPassThrough,
        SKEL_PostOperationPassThrough 
    },
    { 
        IRP_MJ_MDL_READ_COMPLETE,
        0,
        SKEL_PreOperationPassThrough,
        SKEL_PostOperationPassThrough 
    },
    { 
        IRP_MJ_PREPARE_MDL_WRITE,
        0,
        SKEL_PreOperationPassThrough,
        SKEL_PostOperationPassThrough 
    },
    { 
        IRP_MJ_MDL_WRITE_COMPLETE,
        0,
        SKEL_PreOperationPassThrough,
        SKEL_PostOperationPassThrough 
    },
    { 
        IRP_MJ_VOLUME_MOUNT,
        0,
        SKEL_PreOperationPassThrough,
        SKEL_PostOperationPassThrough 
    },
    { 
        IRP_MJ_VOLUME_DISMOUNT,
        0,
        SKEL_PreOperationPassThrough,
        SKEL_PostOperationPassThrough 
    },
    { IRP_MJ_OPERATION_END }
};

CONST FLT_CONTEXT_REGISTRATION  SKEL_ContextRegistration[] = {
	// Per Instance Context
    {
	    FLT_INSTANCE_CONTEXT,
	    0,
	    CtxCleanupCallback,
        SKEL_INSTANCE_CONTEXT_SIZE,
        SKEL_INSTANCE_CONTEXT_TAG,
	    NULL,
	    NULL,
	    0
    },

    // Per stream context

    // Per file handle context

    // Per volume context

};

//
//  This defines what we want to filter with FltMgr
//

CONST FLT_REGISTRATION FilterRegistration = {

    sizeof( FLT_REGISTRATION ),         //  Size
    FLT_REGISTRATION_VERSION,           //  Version
    0,                                  //  Flags

    SKEL_ContextRegistration,           //  Context
    Callbacks,                          //  Operation callbacks

    SKEL_Unload,                           //  MiniFilterUnload

    SKEL_InstanceSetup,                    //  InstanceSetup
    SKEL_InstanceQueryTeardown,            //  InstanceQueryTeardown
    SKEL_InstanceTeardownStart,            //  InstanceTeardownStart
    SKEL_InstanceTeardownComplete,         //  InstanceTeardownComplete

    NULL,                               //  GenerateFileName
    NULL,                               //  GenerateDestinationFileName
    NULL                                //  NormalizeNameComponent

};

//////////////////////////////////////////////////////////////////////////

NTSTATUS SKEL_InstanceSetup( _In_ PCFLT_RELATED_OBJECTS FltObjects,
    _In_ FLT_INSTANCE_SETUP_FLAGS Flags,
    _In_ DEVICE_TYPE VolumeDeviceType,
    _In_ FLT_FILESYSTEM_TYPE VolumeFilesystemType )
{
    NTSTATUS                status;
    PSKEL_INSTANCE_CONTEXT  ctx = NULL;

    UNREFERENCED_PARAMETER( FltObjects );
    UNREFERENCED_PARAMETER( Flags );
    UNREFERENCED_PARAMETER( VolumeDeviceType );
    UNREFERENCED_PARAMETER( VolumeFilesystemType );

    PAGED_CODE();

    if (VolumeFilesystemType == FLT_FSTYPE_RAW || VolumeFilesystemType == FLT_FSTYPE_UNKNOWN) {
        // no point
        return STATUS_FLT_DO_NOT_ATTACH;
    }

	status = FltAllocateContext( FltObjects->Filter, 
        FLT_INSTANCE_CONTEXT, 
        SKEL_INSTANCE_CONTEXT_SIZE, 
        NonPagedPool, 
        (PFLT_CONTEXT*)&ctx);

	if(!NT_SUCCESS(status)) 
	{
        return STATUS_FLT_DO_NOT_ATTACH;
	}

    // ensure zeroed memory (pre win11 didnt)
	RtlZeroMemory(ctx, SKEL_INSTANCE_CONTEXT_SIZE);

    ExInitializeResourceLite(&ctx->Resource);

	status = FltSetInstanceContext(FltObjects->Instance,
		FLT_SET_CONTEXT_KEEP_IF_EXISTS,
		ctx,
		NULL);

    if (!NT_SUCCESS(status)) {
        // free the allocated resource
        FltReleaseContext(ctx);
        return STATUS_FLT_DO_NOT_ATTACH;
    }

    // Store something in the context
    ctx->Flags = Flags;

    return STATUS_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////

NTSTATUS SKEL_InstanceQueryTeardown( _In_ PCFLT_RELATED_OBJECTS FltObjects,
    _In_ FLT_INSTANCE_QUERY_TEARDOWN_FLAGS Flags )
{
    UNREFERENCED_PARAMETER( FltObjects );
    UNREFERENCED_PARAMETER( Flags );

    PAGED_CODE();

    return STATUS_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////

VOID SKEL_InstanceTeardownStart( _In_ PCFLT_RELATED_OBJECTS FltObjects,
    _In_ FLT_INSTANCE_TEARDOWN_FLAGS Flags )
{
    UNREFERENCED_PARAMETER( FltObjects );
    UNREFERENCED_PARAMETER( Flags );

    PAGED_CODE();
}

//////////////////////////////////////////////////////////////////////////

VOID SKEL_InstanceTeardownComplete( _In_ PCFLT_RELATED_OBJECTS FltObjects,
    _In_ FLT_INSTANCE_TEARDOWN_FLAGS Flags )
{
    NTSTATUS    status;
    PSKEL_INSTANCE_CONTEXT  ctx = NULL;

    UNREFERENCED_PARAMETER( FltObjects );
    UNREFERENCED_PARAMETER( Flags );

    PAGED_CODE();

	status = FltGetInstanceContext(FltObjects->Instance, (PFLT_CONTEXT*)&ctx);

    if (NT_SUCCESS(status))
    {
        CtxAcquireResourceExclusive(&ctx->Resource);
            // Do something
        CtxReleaseResource(&ctx->Resource);

        FltReleaseContext(ctx);
    }
}


//////////////////////////////////////////////////////////////////////////

NTSTATUS DriverEntry( _In_ PDRIVER_OBJECT DriverObject, _In_ PUNICODE_STRING RegistryPath )
{
    NTSTATUS status;

    UNREFERENCED_PARAMETER( RegistryPath );


    //  Register with FltMgr to tell it our callback routines
    status = FltRegisterFilter( DriverObject,
                                &FilterRegistration,
                                &gFilterHandle );

    FLT_ASSERT( NT_SUCCESS( status ) );

    if (NT_SUCCESS( status )) {
        //  Start filtering i/o
        status = FltStartFiltering( gFilterHandle );

        if (!NT_SUCCESS( status )) {
            FltUnregisterFilter( gFilterHandle );
        }
    }

    return status;
}

//////////////////////////////////////////////////////////////////////////

NTSTATUS SKEL_Unload( _In_ FLT_FILTER_UNLOAD_FLAGS Flags )
{
    UNREFERENCED_PARAMETER( Flags );

    PAGED_CODE();

    FltUnregisterFilter( gFilterHandle );

    return STATUS_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////

FLT_PREOP_CALLBACK_STATUS SKEL_PreOperationPassThrough( _Inout_ PFLT_CALLBACK_DATA Data,
    _In_ PCFLT_RELATED_OBJECTS FltObjects,
    _Flt_CompletionContext_Outptr_ PVOID *CompletionContext )
{
    NTSTATUS status;

    UNREFERENCED_PARAMETER( FltObjects );
    UNREFERENCED_PARAMETER( CompletionContext );

    //
    //  See if this is an operation we would like the operation status
    //  for.  If so request it.
    //
    //  NOTE: most filters do NOT need to do this.  You only need to make
    //        this call if, for example, you need to know if the oplock was
    //        actually granted.
    //
    if(SKEL_DoRequestOperationStatus( Data ) ) {
        status = FltRequestOperationStatusCallback( Data,
                                                    SKEL_OperationStatusCallback,
                                                    (PVOID)(++OperationStatusCtx) );
        if( !NT_SUCCESS(status) ) {
            // failed
        }
    }

    return FLT_PREOP_SUCCESS_WITH_CALLBACK;
}

//////////////////////////////////////////////////////////////////////////

VOID SKEL_OperationStatusCallback( _In_ PCFLT_RELATED_OBJECTS FltObjects,
    _In_ PFLT_IO_PARAMETER_BLOCK ParameterSnapshot,
    _In_ NTSTATUS OperationStatus,
    _In_ PVOID RequesterContext )
{
    UNREFERENCED_PARAMETER( FltObjects );
    UNREFERENCED_PARAMETER(ParameterSnapshot);
    UNREFERENCED_PARAMETER(OperationStatus);
    UNREFERENCED_PARAMETER(RequesterContext);
}

//////////////////////////////////////////////////////////////////////////

FLT_POSTOP_CALLBACK_STATUS SKEL_PostOperationPassThrough( _Inout_ PFLT_CALLBACK_DATA Data,
    _In_ PCFLT_RELATED_OBJECTS FltObjects,
    _In_opt_ PVOID CompletionContext,
    _In_ FLT_POST_OPERATION_FLAGS Flags )
{
    UNREFERENCED_PARAMETER( Data );
    UNREFERENCED_PARAMETER( FltObjects );
    UNREFERENCED_PARAMETER( CompletionContext );
    UNREFERENCED_PARAMETER( Flags );

    return FLT_POSTOP_FINISHED_PROCESSING;
}

//////////////////////////////////////////////////////////////////////////

FLT_PREOP_CALLBACK_STATUS SKEL_PreOperationNoPostOperationPassThrough( _Inout_ PFLT_CALLBACK_DATA Data,
    _In_ PCFLT_RELATED_OBJECTS FltObjects,
    _Flt_CompletionContext_Outptr_ PVOID *CompletionContext )
{
    UNREFERENCED_PARAMETER( Data );
    UNREFERENCED_PARAMETER( FltObjects );
    UNREFERENCED_PARAMETER( CompletionContext );

    return FLT_PREOP_SUCCESS_NO_CALLBACK;
}

//////////////////////////////////////////////////////////////////////////

BOOLEAN SKEL_DoRequestOperationStatus( _In_ PFLT_CALLBACK_DATA Data )
{
    PFLT_IO_PARAMETER_BLOCK iopb = Data->Iopb;

    //  return boolean state based on which operations we are interested in
    return (BOOLEAN)
            //  Check for oplock operations
             (((iopb->MajorFunction == IRP_MJ_FILE_SYSTEM_CONTROL) &&
               ((iopb->Parameters.FileSystemControl.Common.FsControlCode == FSCTL_REQUEST_FILTER_OPLOCK)  ||
                (iopb->Parameters.FileSystemControl.Common.FsControlCode == FSCTL_REQUEST_BATCH_OPLOCK)   ||
                (iopb->Parameters.FileSystemControl.Common.FsControlCode == FSCTL_REQUEST_OPLOCK_LEVEL_1) ||
                (iopb->Parameters.FileSystemControl.Common.FsControlCode == FSCTL_REQUEST_OPLOCK_LEVEL_2)))

              ||

              //    Check for directy change notification
              ((iopb->MajorFunction == IRP_MJ_DIRECTORY_CONTROL) &&
               (iopb->MinorFunction == IRP_MN_NOTIFY_CHANGE_DIRECTORY))
             );
}

//////////////////////////////////////////////////////////////////////////

