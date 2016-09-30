#Arm MCU Plugin For QT Creator
#QT Creator Version=4.02
#toolchain="arm-none-eabi-gcc" + "openocd" + "qbs" or "scons"
#My Email:snikeguo@foxmail.com
#初版完成，目前没解决的问题是 1.如果想添加文件的话，用户得手动修改prj.qbs文件。2.如果是IAP程序，我们知道要修改ROM的起始地址，目前还只能在ld文件里修改。暂时还没集成到插件里，所以插件修改地址的那个是灰色的。
#说明。此插件先解析KEIL的工程。得到参数，由于KEIL默认的编译器是ARMCC，所以启动文件（比如startup_STM32F103XX.s）是ARMCC编译器的。用户要替换成GCC编#译器的启动文件。
#还有链接脚本文件（ld文件，也是要找到GCC版本的。然后添加进来）
#Q：STM32如何得到启动文件和链接脚本文件？
#A：请用CUBEMX 生成SW4STM32工程或者生成TrueStudio工程。即可得到链接脚本和启动文件。然后添加链接脚本，吧ARMCC的启动文件去掉，添加进来GCC的启动文件。对于其他芯片，厂商的库一般都是提供了的。自己在网上找一下就可以了。

#下次更新的看点：
#修复以上问题。使得插件完成全部的功能，但除了启动文件的替换。链接脚本添加进来的话，会自动解析链接脚本，就可以直接在窗体中修改ROM的地址，从而更新到LD文件。
#定制调试协议，对于带RTOS的来说，此插件提供了一个类似于windows下的任务管理器的窗口。 在调试的时候实时监测各个任务的运行状态。有打算动态修改优先级，当然这是后话了。
#支持的RTOS有：CMSIS-RTOS标准的OS， RAWOS。
#2019.9.30

#Signed-off-by: David <snikeguo@foxmail.com>

