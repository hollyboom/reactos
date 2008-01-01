/* $Id$
 *
 * COPYRIGHT:       See COPYING in the top level directory
 * PROJECT:         ReactOS kernel
 * FILE:            ntoskrnl/cc/fs.c
 * PURPOSE:         Implements MDL Cache Manager Functions
 *
 * PROGRAMMERS:     Alex Ionescu
 */

/* INCLUDES ******************************************************************/

#include <ntoskrnl.h>
#define NDEBUG
#include <internal/debug.h>

/* FUNCTIONS *****************************************************************/

/*
 * @implemented
 */
VOID STDCALL
CcMdlRead (IN PFILE_OBJECT FileObject,
           IN PLARGE_INTEGER FileOffset, 
           IN ULONG Length, 
           OUT PMDL * MdlChain, 
           OUT PIO_STATUS_BLOCK IoStatus)
{
    UNIMPLEMENTED;
}

/*
 * NAME                            INTERNAL
 * CcMdlReadCompleteDev@8
 *
 * DESCRIPTION
 *
 * ARGUMENTS
 * MdlChain
 * DeviceObject
 *
 * RETURN VALUE
 *  None.
 *
 * NOTE
 *     Used by CcMdlReadComplete@8 and FsRtl
 *
 */
VOID STDCALL
CcMdlReadCompleteDev (IN PMDL MdlChain, 
                      IN PFILE_OBJECT FileObject)
{
    PMDL Mdl;

    /* Free MDLs */
    while ((Mdl = MdlChain))
    {
        MdlChain = Mdl->Next;
        MmUnlockPages (Mdl);
        IoFreeMdl (Mdl);
    }
}

/*
 * NAME    EXPORTED
 * CcMdlReadComplete@8
 *
 * DESCRIPTION
 *
 * ARGUMENTS
 *
 * RETURN VALUE
 * None.
 *
 * NOTE
 * From Bo Branten's ntifs.h v13.
 *
 * @implemented
 */
VOID STDCALL
CcMdlReadComplete (IN PFILE_OBJECT FileObject, 
                   IN PMDL MdlChain)
{
    PDEVICE_OBJECT DeviceObject = NULL;
    PFAST_IO_DISPATCH FastDispatch;

    /* Get Fast Dispatch Data */
    DeviceObject = IoGetRelatedDeviceObject (FileObject);
    FastDispatch = DeviceObject->DriverObject->FastIoDispatch;

    /* Check if we support Fast Calls, and check this one */
    if (FastDispatch && FastDispatch->MdlReadComplete)
    {
        /* Use the fast path */
        FastDispatch->MdlReadComplete (FileObject, MdlChain, DeviceObject);
    }

    /* Use slow path */
    CcMdlReadCompleteDev (MdlChain, FileObject);
}

/*
 * @implemented
 */
VOID STDCALL
CcMdlWriteComplete (IN PFILE_OBJECT FileObject, 
                    IN PLARGE_INTEGER FileOffset, 
                    IN PMDL MdlChain)
{
    PDEVICE_OBJECT DeviceObject = NULL;
    PFAST_IO_DISPATCH FastDispatch;

    /* Get Fast Dispatch Data */
    DeviceObject = IoGetRelatedDeviceObject (FileObject);
    FastDispatch = DeviceObject->DriverObject->FastIoDispatch;

    /* Check if we support Fast Calls, and check this one */
    if (FastDispatch && FastDispatch->MdlWriteComplete)
    {
        /* Use the fast path */
        FastDispatch->MdlWriteComplete (FileObject, FileOffset, MdlChain, DeviceObject);
    }

    /* Use slow path */
    CcMdlWriteCompleteDev (FileObject, FileOffset, MdlChain);
}

/*
 * @unimplemented
 */
VOID NTAPI
CcMdlWriteCompleteDev (IN PFILE_OBJECT FileObject, 
                       IN PLARGE_INTEGER FileOffset, 
                       IN PMDL MdlChain)
{
    UNIMPLEMENTED;
}

/*
 * @unimplemented
 */
VOID STDCALL
CcMdlWriteAbort (IN PFILE_OBJECT FileObject, 
                 IN PMDL MdlChain)
{
    UNIMPLEMENTED;
}

/*
 * @unimplemented
 */
VOID STDCALL
CcPrepareMdlWrite (IN PFILE_OBJECT FileObject,
                   IN PLARGE_INTEGER FileOffset, 
                   IN ULONG Length, 
                   OUT PMDL * MdlChain, 
                   OUT PIO_STATUS_BLOCK IoStatus)
{
    UNIMPLEMENTED;
}
