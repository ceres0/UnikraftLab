BRIDGE_IFACE="virbr0"
BRIDGE_IP="172.44.0.1"

sudo ip link set dev "$BRIDGE_IFACE" down 2> /dev/null
sudo ip link del dev "$BRIDGE_IFACE" 2> /dev/null
sudo ip link add dev "$BRIDGE_IFACE" type bridge
sudo ip address add "$BRIDGE_IP"/24 dev "$BRIDGE_IFACE"
sudo ip link set dev "$BRIDGE_IFACE" up
