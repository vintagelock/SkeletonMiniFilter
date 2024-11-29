
#pragma once

//////////////////////////////////////////////////////////////////////////

typedef struct _tSKEL_INSTANCE_CONTEXT
{
	ERESOURCE	Resource;
	ULONG		Flags;
}SKEL_INSTANCE_CONTEXT, * PSKEL_INSTANCE_CONTEXT;

#define SKEL_INSTANCE_CONTEXT_SIZE		sizeof( SKEL_INSTANCE_CONTEXT )
#define SKEL_INSTANCE_CONTEXT_TAG		'sk0a'

//////////////////////////////////////////////////////////////////////////

FORCEINLINE VOID CtxAcquireResourceExclusive(PERESOURCE Resource)
{
	FLT_ASSERT(KeGetCurrentIrql() <= APC_LEVEL);
	FLT_ASSERT(ExIsResourceAcquiredExclusiveLite(Resource) || !ExIsResourceAcquiredSharedLite(Resource));

	KeEnterCriticalRegion();
	(VOID)ExAcquireResourceExclusiveLite(Resource, TRUE);
}

FORCEINLINE VOID CtxAcquireResourceShared(PERESOURCE Resource)
{
	FLT_ASSERT(KeGetCurrentIrql() <= APC_LEVEL);

	KeEnterCriticalRegion();
	(VOID)ExAcquireResourceSharedLite(Resource, TRUE);
}

FORCEINLINE VOID CtxReleaseResource(IN PERESOURCE Resource)
{
	FLT_ASSERT(KeGetCurrentIrql() <= APC_LEVEL);
	FLT_ASSERT(ExIsResourceAcquiredExclusiveLite(Resource) || ExIsResourceAcquiredSharedLite(Resource));

	ExReleaseResourceLite(Resource);
	KeLeaveCriticalRegion();
}

FORCEINLINE VOID CtxFreeResource(IN PERESOURCE Resource)
{
	ExFreePool(Resource);
}

PERESOURCE CtxAllocResource(ULONG ulTag);
VOID CtxCleanupCallback(IN PFLT_CONTEXT Context, IN FLT_CONTEXT_TYPE ContextType);

