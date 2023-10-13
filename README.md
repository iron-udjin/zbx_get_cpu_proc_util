# Tool for calculating CPU utilization by processes for Zabbix on FreeBSD

Tested on **Zabbix 6.0 LTS** and **FreeBSD 13.2-RELEASE**.

# Install
* Place `zabbix_get_cpu_proc_util.conf` and `proc_cpu_util.c` to `/usr/local/etc/zabbix6/zabbix_agentd.conf.d`
* Compille source:
  ```
  cc -Weverything -lutil -lm -Ofast -march=native -flto proc_cpu_util.c -o proc_cpu_util
  ```
* Make sure you have: `Include=/usr/local/etc/zabbix6/zabbix_agentd.conf.d/*.conf` in your `zabbix_agentd.conf`
* Restart Zabbix Agent: `service zabbix_agentd restart`

# Testing
```
zabbix_get -s <zabbix agent IP> -k 'proc.cpu.util["php-fpm"]'
```
Replace `php-fpm` by any other existing process name.

# How does it work
The tool performs a query for the full process list, filters it by process name, and summarizes CPU utilization for all threads/processes. It assumes that 100% CPU usage occurs when all CPU threads have a 100% load. For example, if a process utilizes 100% of two CPU cores on hardware with 16 CPUs, that means the process utilizes `2 * 100 / 16 = 12%` of the CPU.

# Zabbix templates?
No templates. Use existing like `PHP-FPM by Zabbix agent`/`App Varnish by Zabbix agent` or make your own which will call `proc.cpu.util["{$PROCESS_NAME}"]`.
