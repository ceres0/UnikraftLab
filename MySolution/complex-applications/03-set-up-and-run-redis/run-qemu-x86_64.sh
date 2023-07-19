#!/bin/bash

if test $# -ne 1; then
    echo "Usage: $0 <path_to_kvm_image>" 1>&2
    exit 1
fi

KVM_IMAGE="$1"

sudo ip link set dev virbr0 down 2> /dev/null
sudo ip link del dev virbr0 2> /dev/null
sudo ip link add dev virbr0 type bridge
sudo ip address add 172.44.0.1/24 dev virbr0
sudo ip link set dev virbr0 up


sudo dnsmasq -d \
             -log-queries \
             --bind-dynamic \
             --interface=virbr0\
             --listen-addr=172.44.0.1 \
             --dhcp-range=172.44.0.2,172.44.0.254,255.255.255.0,12h &> dnsmasq.logs &

sudo /usr/bin/qemu-system-x86_64 \
    -fsdev local,id=myid,path=$(pwd)/fs0,security_model=none \
    -device virtio-9p-pci,fsdev=myid,mount_tag=fs0,disable-modern=on,disable-legacy=off \
    -netdev bridge,id=en0,br=virbr0 -device virtio-net-pci,netdev=en0 \
    -append "/redis.conf" \
    -kernel "$KVM_IMAGE" -nographic
