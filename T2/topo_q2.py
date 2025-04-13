from mininet.topo import Topo
from mininet.net import Mininet
from mininet.node import Node
from mininet.link import TCLink
from mininet.cli import CLI
from mininet.log import setLogLevel

class Router(Node):
    """Router node with IP forwarding enabled"""
    def config(self, **params):
        super().config(**params)
        self.cmd('sysctl -w net.ipv4.ip_forward=1')

class CustomNATTopo(Topo):
    def build(self):
        # Define switches
        internalSwitch = self.addSwitch('s1')
        externalSwitch = self.addSwitch('s2')

        # Define internal hosts
        internalHost1 = self.addHost('h1', ip='10.1.1.2/24', defaultRoute='via 10.1.1.1')
        internalHost2 = self.addHost('h2', ip='10.1.1.3/24', defaultRoute='via 10.1.1.1')

        # Define external hosts
        for i in range(3, 9):
            hostName = f'h{i}'
            hostIP = f'10.0.0.{i+1}/24'
            self.addHost(hostName, ip=hostIP)

        # Add router node acting as NAT
        natRouter = self.addNode('nat0', cls=Router, ip='10.1.1.1/24')

        # Internal connections
        self.addLink(internalHost1, internalSwitch)
        self.addLink(internalHost2, internalSwitch)
        self.addLink(natRouter, internalSwitch)

        # External connections
        self.addLink(natRouter, externalSwitch)
        for i in range(3, 9):
            self.addLink(f'h{i}', externalSwitch)

def setup():
    topology = CustomNATTopo()
    network = Mininet(topo=topology, link=TCLink)
    network.start()

    # NAT configuration
    router = network.get('nat0')
    router.cmd('ip addr add 10.0.0.10/24 dev nat0-eth1')
    router.cmd('ip link set nat0-eth1 up')

    # Setting up iptables for NAT
    router.cmd('iptables -t nat -A POSTROUTING -o nat0-eth1 -j MASQUERADE')
    router.cmd('iptables -A FORWARD -i nat0-eth0 -o nat0-eth1 -j ACCEPT')
    router.cmd('iptables -A FORWARD -i nat0-eth1 -o nat0-eth0 -m state --state ESTABLISHED,RELATED -j ACCEPT')

    CLI(network)
    network.stop()

if __name__ == '__main__':
    setLogLevel('info')
    setup()