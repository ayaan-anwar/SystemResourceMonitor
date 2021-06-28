BASE_PATH = "C:\\dev\\projects\\OS_Project\\SystemResourceMonitor\\"

import winstats
import wmi

computer = wmi.WMI()
computer_info = computer.Win32_ComputerSystem()[0]
os_info = computer.Win32_OperatingSystem()[0]
proc_info = computer.Win32_Processor()[0]
gpu_info = computer.Win32_VideoController()[0]

os_name = os_info.Name.encode('utf-8').split(b'|')[0]
os_version = ' '.join([os_info.Version, os_info.BuildNumber])
system_ram = float(os_info.TotalVisibleMemorySize) / 1048576  # KB to GB

usage = winstats.get_perf_data(r'\Processor(_Total)\% Processor Time', delay=100)
# print '    CPU Usage: %.02f %%' % usage

meminfo = winstats.get_mem_info()
total = meminfo.TotalPhys
load = meminfo.MemoryLoad
# TotalPhys, Memory Load

drives = winstats.get_drives()
drive = drives[0]
fsinfo = winstats.get_fs_usage(drive)
vinfo = winstats.get_vol_info(drive)
# print '    Disks:', ', '.join(drives)
# print '    %s:\\' % drive
# print '        Name:', vinfo.name
# print '        Type:', vinfo.fstype
# print '        Total:', fmt(fsinfo.total)
# print '        Used: ', fmt(fsinfo.used)
# print '        Free: ', fmt(fsinfo.free)

# First 5 lines
# print('OS Name: {0}'.format(os_name))
# print('OS Version: {0}'.format(os_version))
# print('CPU: {0}'.format(proc_info.Name))
# print('RAM: {0} GB'.format(system_ram))
# print('Graphics Card: {0}'.format(gpu_info.Name))

with open(BASE_PATH + 'sys_info.txt', 'w') as file:
    # Basic
    file.write('OS Name: {0}\n'.format(os_name))
    file.write('OS Version: {0}\n'.format(os_version))
    file.write('CPU: {0}\n'.format(proc_info.Name))
    file.write('RAM: {0} GB\n'.format(system_ram))
    file.write('Graphics Card: {0}\n'.format(gpu_info.Name))

    # CPU Usage
    file.write(str(usage[0]) + '\n')

    # RAM
    file.write(str(load) + '\n')

    # Storage
    file.write(vinfo.name + '\n')
    file.write(vinfo.fstype + '\n')
    file.write(str(fsinfo.total / 1e9) + '\n')
    file.write(str(fsinfo.used / 1e9) + '\n')
    file.write(str(fsinfo.free / 1e9) + '\n')
