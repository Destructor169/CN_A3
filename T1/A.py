from mininet.net import Mininet
from mininet.node import Controller, OVSKernelSwitch, RemoteController
from mininet.cli import CLI
from mininet.log import setLogLevel, info
from mininet.link import TCLink

def createTopology():
    class OVSSwitchSTP(OVSKernelSwitch):
        def start(self, *args, **kwargs):
            OVSKernelSwitch.start(self, *args, **kwargs)
            self.cmd('ovs-vsctl set bridge', self, 'stp_enable=true')
            # Remove the OpenFlow13 protocol setting as it's causing issues
    
    # Create network with RemoteController
    net = Mininet(
        controller=RemoteController,
        switch=OVSSwitchSTP,
        link=TCLink,
        autoSetMacs=True
    )
    
    # Add controller with specific IP and port
    info('*** Adding controller\n')
    net.addController('c0', controller=RemoteController, ip='127.0.0.1', port=6653)
    
    # Add switches
    info('*** Adding switches\n')
    s1 = net.addSwitch('s1')
    s2 = net.addSwitch('s2')
    s3 = net.addSwitch('s3')
    s4 = net.addSwitch('s4')
    
    # Add hosts
    info('*** Adding hosts\n')
    h1 = net.addHost('h1', ip='10.0.0.2/24')
    h2 = net.addHost('h2', ip='10.0.0.3/24')
    h3 = net.addHost('h3', ip='10.0.0.4/24')
    h4 = net.addHost('h4', ip='10.0.0.5/24')
    h5 = net.addHost('h5', ip='10.0.0.6/24')
    h6 = net.addHost('h6', ip='10.0.0.7/24')
    h7 = net.addHost('h7', ip='10.0.0.8/24')
    h8 = net.addHost('h8', ip='10.0.0.9/24')
    
    # Add links with latency
    info('*** Adding links\n')
    # Switch to switch links (7ms latency)
    net.addLink(s1, s2, delay='7ms')
    net.addLink(s2, s3, delay='7ms')
    net.addLink(s3, s4, delay='7ms')
    net.addLink(s4, s1, delay='7ms')
    net.addLink(s1, s3, delay='7ms')
    
    # Host to switch links (5ms latency)
    net.addLink(h1, s1, delay='5ms')
    net.addLink(h2, s1, delay='5ms')
    net.addLink(h3, s2, delay='5ms')
    net.addLink(h4, s2, delay='5ms')
    net.addLink(h5, s3, delay='5ms')
    net.addLink(h6, s3, delay='5ms')
    net.addLink(h7, s4, delay='5ms')
    net.addLink(h8, s4, delay='5ms')
    
    # Start network
    net.start()
    
    # Enter CLI
    CLI(net)
    
    # Stop network
    net.stop()

if __name__ == '__main__':
    setLogLevel('info')
    createTopology()
