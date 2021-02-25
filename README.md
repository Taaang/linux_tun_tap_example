# linux_tun_tap_example
An example for tun/tap develop, the code try to implement the ICMP request/replay.

## Compile

`gcc tun_example.c`

## Example 

1.  Create `tun` character device file in `/dev/net/`.

```
mknod mmq c 10 200
```

Major and minor devcie code must be 10, 200, linux kernel define it.
(More detail in linux kernel source code document `/admin-gui/devices.txt`)
  
  
  
2. Run the program, it will create a `tun` virtual network device named 'mmq-eth'.

You can find it by `ifconfig -a`.
  
  
  
3. Configurate `tun` virtual network interface , set ip route to the device.

```
ifconfig mmq-eth up
route add -host 10.10.10.1 dev mmq-eth
```



4. `ping` with 10.10.10.1

## Result 

![](https://raw.githubusercontent.com/Taaang/blog/master/assets/images/post_imgs/tuntap/5.png)
