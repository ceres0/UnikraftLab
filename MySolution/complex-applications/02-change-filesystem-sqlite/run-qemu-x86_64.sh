#!/bin/sh

if test $# -ne 1; then
    echo "Usage: $0 <path_to_kvm_image>" 1>&2
    exit 1
fi

KVM_IMAGE="$1"

/usr/bin/qemu-system-x86_64 \
    -fsdev local,id=myid,path="$(pwd)/fs0",security_model=none \
    -device virtio-9p-pci,fsdev=myid,mount_tag=fs0,disable-modern=on,disable-legacy=off \
    -kernel "$KVM_IMAGE" -nographic \
    -initrd archive.cpio
