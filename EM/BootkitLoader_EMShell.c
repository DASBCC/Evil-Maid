#include <efi.h>
#include <efilib.h>

EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
    InitializeLib(ImageHandle, SystemTable);

    EFI_LOADED_IMAGE *LoadedImage;
    EFI_GUID LoadedImageProtocol = LOADED_IMAGE_PROTOCOL;
    EFI_STATUS Status = uefi_call_wrapper(BS->HandleProtocol, 3,
                                          ImageHandle, &LoadedImageProtocol, (VOID **)&LoadedImage);
    if (EFI_ERROR(Status)) return Status;

    EFI_GUID FileSystemProtocol = SIMPLE_FILE_SYSTEM_PROTOCOL;
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *FileSystem;
    Status = uefi_call_wrapper(BS->HandleProtocol, 3,
                               LoadedImage->DeviceHandle, &FileSystemProtocol, (VOID **)&FileSystem);
    if (EFI_ERROR(Status)) return Status;

    EFI_FILE_PROTOCOL *Root;
    Status = uefi_call_wrapper(FileSystem->OpenVolume, 2, FileSystem, &Root);
    if (EFI_ERROR(Status)) return Status;

    EFI_FILE_PROTOCOL *File;
    Status = uefi_call_wrapper(Root->Open, 5, Root, &File, L"EFI/BOOT/EMShell.bin", EFI_FILE_MODE_READ, 0);
    if (EFI_ERROR(Status)) return Status;

    UINTN BufferSize = 0x20000;  // 128KB
    void *Buffer;
    Status = uefi_call_wrapper(BS->AllocatePool, 3, EfiLoaderData, BufferSize, &Buffer);
    if (EFI_ERROR(Status)) return Status;

    Status = uefi_call_wrapper(File->Read, 3, File, &BufferSize, Buffer);
    uefi_call_wrapper(File->Close, 1, File);
    if (EFI_ERROR(Status)) return Status;

    // Optionally do something with Buffer (the EMShell.bin payload)

    EFI_DEVICE_PATH *DevicePath = FileDevicePath(LoadedImage->DeviceHandle,
                                                 L"EFI/Microsoft/Boot/bootmgfw.efi");
    EFI_HANDLE BootHandle;
    Status = uefi_call_wrapper(BS->LoadImage, 6, FALSE, ImageHandle, DevicePath, NULL, 0, &BootHandle);
    if (EFI_ERROR(Status)) return Status;

    Status = uefi_call_wrapper(BS->StartImage, 3, BootHandle, NULL, NULL);
    return Status;
}
