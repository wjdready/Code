import socket
from scapy.all import ARP, Ether, srp
from concurrent.futures import ThreadPoolExecutor

def get_mac(ip):
    arp = ARP(pdst=ip)
    ether = Ether(dst="ff:ff:ff:ff:ff:ff")
    packet = ether/arp
    result = srp(packet, timeout=1, verbose=0)[0]  # 减少超时时间
    if result:
        return result[0][1].hwsrc
    return None

def get_hostname(ip):
    try:
        return socket.gethostbyaddr(ip)[0]
    except socket.herror:
        return None

def scan_single_ip(ip):
    mac = get_mac(ip)
    if mac:
        hostname = get_hostname(ip)
        print(f"正在扫描 IP: {ip}", flush=True)
        return {'ip': ip, 'mac': mac, 'hostname': hostname}
    return None

def scan_network(ip_range):
    # 生成 IP 列表
    ip_parts = ip_range.split('/')
    base_ip = ip_parts[0]
    mask = int(ip_parts[1])
    num_ips = 2 ** (32 - mask)
    start_ip = list(map(int, base_ip.split('.')))
    ips = []
    for i in range(num_ips):
        ip = start_ip.copy()
        ip[3] = (start_ip[3] + i) & 0xFF
        if ip[3] == 0 or ip[3] == 255:  # 跳过网络地址和广播地址
            continue
        ips.append('.'.join(map(str, ip)))

    clients = []
    # 使用线程池并发扫描
    with ThreadPoolExecutor(max_workers=100) as executor:
        results = executor.map(scan_single_ip, ips)
        for result in results:
            if result:
                clients.append(result)
    return clients

if __name__ == "__main__":
    # 请根据实际局域网网段修改，常见的如 192.168.1.0/24 或 192.168.0.0/24
    ip_range = "192.168.0.0/24"
    clients = scan_network(ip_range)

    print("IP 地址" + " " * 15 + "MAC 地址" + " " * 15 + "主机名")
    for client in clients:
        print(f"{client['ip']:16} {client['mac']:20} {client['hostname'] if client['hostname'] else '未找到'}")
