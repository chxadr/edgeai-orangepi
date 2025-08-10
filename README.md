<div align="center">

# Edge AI with Orange Pi 🍊

<img src=assets/images/setup.jpg  width=530>

</div>

## 🎯 Content

This repository presents **a minimalistic detection system** capable of **recognizing specific characters** from John Carpenter’s *They Live* (1988) — namely John Nada and the aliens — and **targeting** the latter **by controlling a laser**.

The system was developed within a **strict four-week timeframe** to demonstrate the feasibility of deploying a **complete real-time embedded solution** based on a **lightweight deep learning model**. This model performs common computer vision tasks such as **classification and detection**, with its results feeding into further post-processing to specialize the system. The project highlights how such a setup can be achieved using limited resources and a relatively simple configuration.

Our prototype is based on an [Orange Pi Zero 3](http://www.orangepi.org/html/hardWare/computerAndMicrocontrollers/details/Orange-Pi-Zero-3.html) single-board computer, connected to a camera feeding a [YOLOv8n](https://docs.ultralytics.com/models/yolov8/) AI detection model, and enhanced with a [Coral USB Accelerator](https://coral.ai/products/accelerator) for better performances without sacrificing its low power consumption footprint.

This document outlines:

- The **system and its components**, including the **technical decisions** made along with their **advantages and limitations**;
- The **hardware and software configuration** of the system's elements.

> [!TIP]
> Please check the submodules of this repository for further precisions on *what models we worked* on and *how we build the dataset* ([humanxalien-dataset](humanxalien-dataset/)); and *how the model has been trained* ([yoloxcoral](yoloxcoral/)).

### 📜 License

The source code in this repository is licensed under the [GNU Affero General Public License v3.0 or later](LICENSE).

### ✨ Credits

I would like to thank [**Selva Systems**](https://selvasystems.net/), where I completed my internship, for providing the resources that allowed me to develop the system, from capturing the dataset to training and testing a [YOLOv8n](https://docs.ultralytics.com/models/yolov8/) model, and integrating it onto the [Orange Pi Zero 3](http://www.orangepi.org/html/hardWare/computerAndMicrocontrollers/details/Orange-Pi-Zero-3.html) single-board computer for deployment with the [Coral USB Accelerator](https://coral.ai/products/accelerator).

## System Overview

The concept is to continuously **capture images using a camera** and send them through a **CNN-based detection model** for inference. For each image, the model would **identify which characters are present** and **outputs the position of any detected alien**. These positions would be then used to **control two stepper motors**: one for the horizontal axis (X) and one for the vertical axis (Y), each equipped with a **mirror to redirect a laser beam toward alien targets**.

All components would be controlled programmatically, with the goal of achieving this functionality while consuming as little power as possible.

<div align="center">

<br>
<img src=assets/images/system.png  width=500>

***Figure 1:** System block diagram.*

</div>

To meet these requirements, the device must offer enough computational power to centralize the control of all components within a multithreaded application, yet maintain a low power consumption footprint. For this reason, we chose a single-board computer (SBC) as the system’s central controller — specifically, the [Orange Pi Zero 3](http://www.orangepi.org/html/hardWare/computerAndMicrocontrollers/details/Orange-Pi-Zero-3.html).

### 📷 Camera

We use a **2MP USB camera equipped with a 2.8-12mm lens and a variable focal length.** The focus is adjusted manually. The camera supports **MJPG capture mode at a resolution of 320×240 in which it can record at up to 120 frames per second.** Since many CNNs process images at resolutions that are multiples of 32×32, this camera will allow us to capture dataset images at a resolution of 224×224.

**This resolution is well-suited for our application**, as the John and alien characters differ significantly in texture and color. Given that **we don't require more than 1mm precision** with the laser beam to reach the detected characters' centers, this resolution offers a good balance between accuracy and computational efficiency. **It will also help reduce the complexity of our model**, as the resolution of the training images directly affects the density of the network layers and sometimes the number of hyperparameters.

### 📟 Orange Pi Zero 3

The [Orange Pi Zero 3](http://www.orangepi.org/html/hardWare/computerAndMicrocontrollers/details/Orange-Pi-Zero-3.html) is a **compact, low-power, and versatile SBC** (50×55mm) featuring an Allwinner H618 Quad-core Cortex-A53 processor running at 1.5GHz, along with 4GB of LPDDR4 RAM. It includes two expansion interfaces with GPIOs and additional connectors (USB 2.0/3.0, SPI, I²C, UART), alongside pre-soldered connectors (USB 2.0, Micro HDMI, USB-C power supply). The board supports network connectivity via a Gigabit LAN port or Wi-Fi 5, and also features Bluetooth 5.0. Its overall power consumption can be kept under 5W while in use.

We are using this board because the company already owns one. However, **it lacks the necessary computational power for lightweight deep learning networks.** Newer boards, such as the [Orange Pi 5](http://www.orangepi.org/html/hardWare/computerAndMicrocontrollers/details/Orange-Pi-5.html) with Rockchip microcontrollers or the [Orange Pi RV2](http://www.orangepi.org/html/hardWare/computerAndMicrocontrollers/details/Orange-Pi-RV2.html) featuring an 8-core RISC-V AI CPU, offer significantly more processing power. Despite their increased capabilities, their power consumption remains comparable to that of the [Orange Pi Zero 3](http://www.orangepi.org/html/hardWare/computerAndMicrocontrollers/details/Orange-Pi-Zero-3.html) (5W-15W), especially considering the enhanced connectivity and hardware features they provide.

#### Coral USB Accelerator

To **enhance the performance of our SBC**, we integrated a [Coral USB Accelerator](https://coral.ai/products/accelerator). **This device adds a Google Edge TPU coprocessor to the system** via a USB 3.0 connection. **The Edge TPU is capable of performing 4 tera-operations per second (TOPS), consuming only 0.5 watts per TOPS (2 TOPS/W).** [This benchmark](https://coral.ai/docs/edgetpu/benchmarks/) presents the time per inference results for different AI models, both using the Quad-core Cortex-A53 CPU alone and in combination with the Edge TPU. As the benchmark shows, the gain in performance is impressive, though it can vary based on implementation (e.g., C++ vs Python) and factors like board load or how well the application is developed. Nonetheless, it gives us a strong indication of what to expect.

#### Operating System

The [Orange Pi Zero 3](http://www.orangepi.org/html/hardWare/computerAndMicrocontrollers/details/Orange-Pi-Zero-3.html) is **compatible with modified versions of standard GNU/Linux distributions like Debian and Arch**, based on **kernel versions 5.4 or 6.1**. These **kernels are re-compiled to support hardware functionalities** such as PWM signal generation and serial communication. However, they are **not patched with the real-time branch**, which can limit priority handling.

It is important to note that **even with the real-time kernel patch, strict real-time capabilities are not achievable**. This is because such operating systems are not designed for running a single specific application, but rather for a wide range of tasks. **Consequently, the use of hardware interrupts or timers is quite limited and not very efficient.** 

Therefore, **one might consider using an external microcontroller** like an [ESP32](https://www.espressif.com/en/products/socs/esp32) to interact with the hardware, a choice we would have made if time had permitted after we discovered the limitiations of regular operating systems.

Despite these limitations, the **provided GNU/Linux distributions are useful for running multithreaded applications and managing external devices** such as screens and SD card readers. They also offer the advantage of a **file system and the ability to create services** using `systemd`.

### 🔍 Detection Model

We selected the [Ultralytics](https://www.ultralytics.com/) [YOLOv8n](https://docs.ultralytics.com/models/yolov8/) model as our **detection model**. This nano version of the YOLOv8 model is the **lightest and offers decent precision with real-time inference capabilities**. Its training pipeline is straightforward to implement, using the Python API, and includes **advanced data augmentation strategies** such as mosaic, enabling users to **train models even on small and incomplete datasets.**

**YOLO** (*You Only Look Once*) **can detect multiple instances in a single frame by processing the image in a single forward pass**. For each frame, it predicts classes and bounding boxes along with their confidence scores. The model relies on a CNN backbone to extract spatial features from the image. The neck then prepares the feature maps for three different detection stages that process features of varying abstraction levels. The head of the network consists of these detection stages, which include two decoupled heads: one for class prediction and one for bounding box predictions. This eliminates the need for anchors in the split.

**YOLO models have the ability to accept images of any dimension as input**, even though training is typically done on images of fixed size which influences the complexity of the network. This is achieved through a dedicated spatial pyramid pooling block that decomposes feature maps of various sizes into numerous subsamples of known fixed size.

At the end of the training process, **users have the option to export the model in various formats, including the one compatible with Google Edge TPU**, which requires an `int8` quantized TensorFlow Lite format.

Finally, **the model can be utilized using the Ultralytics Python API**, which notably handles the loading of the model into the [Coral USB Accelerator](https://coral.ai/products/accelerator) memory. It is worth noting that Ultralytics does not provide an official C++ API. We attempted to run our model within a C++ thread without success, so we opted to use a Python thread for the specific task of running inferences.

### 🔦 Laser Targeting

We **redirect a laser beam toward detected aliens using two mirrors**, each one fixed to a NEMA 17 **stepper motor** driven by an A4988 controller. These motors have an angular resolution of 0.9°/step, which make them reasonably precise considering a target at 40cm from the light source, given the fact that the A4988 controllers offer the ability to configure the usage of microsteps (1/2, 1/4 and 1/8 of full step).

It is worht mentionning that **stepper motors require a microcontroller which is able to generated PWM signals** to work properly. You could also generate your own PWM signal using hardware timers.

Stepper motors can present **issues related to torque and manufacturing quality, often resulting in missed steps**. This can cause the system to lose precision over time, **leading to a discrepancy between the requested target coordinates and the actual position of the beam**. One potential solution is to gradually increase the motor speed when movement is required by linearly increasing the frequency of the PWM signal, a technique commonly referred to as **ramp control**. While ramp control can effectively manage torque, it **tends to slow down the system**.

Another challenge is the **difficulty in controlling the number of steps on non-real-time operating systems**. These systems do not guarantee that a thread managing a PWM signal will resume precisely on time to stop the signal and achieve the exact number of steps, which can result in step jumps.

**A more effective, albeit more expensive, alternative for controlling the mirrors would be to use galvanometer motors.**

### 💽 Application

The system's software has been developed as a **multithreaded application**:

- 1× C++ thread using Open CV to capture frames from the camera;
- 1× Python thread to run inference on the captured frames using the YOLOv8n model;
- 2× C threads for controlling the two stepper motors;
- 1× optional C++ thread for displaying the annoted frames from YOLOv8n on screen, featuring drawn colored bounding boxes.

The application is launched through a Python script. **The C/C++ instructions are compiled into a dynamic library** that the Python script and thread can use to communicate with C/C++ threads.

## Orange Pi Zero 3 Configuration

Use Ubuntu 22.02 (Jammy) modified image available at <http://www.orangepi.org/html/hardWare/computerAndMicrocontrollers/service-and-support/Orange-Pi-Zero-3.html>

### Time Synchronization

System clock need to be synchronized for the OrangePi to access the Internet, for example to perform updates.

```
$ sudo systemctl enable chrony
$ sudo systemctl start chrony
$ chronyc sources
```

You will see mutliple NTP servers available. If you have `^?` under the column `MS`, of the output table, it means the OrangePi is not synchronized.

Try to ping a server, here `pool.ntp.org` as an example:

```
$ ping -c 3 pool.ntp.org
```

Try all the server you have. If you cannot ping these servers, the OrangePi is likely not connected to a WAN. Try to reconnect to your network with your credentials. Then restart chrony:

```
$ sudo systemctl restart chrony
```

Wait a few seconds and run `chronyc sources` again. You should see symboles like:

- `^*`  current sync source
- `^+`  good alternative
- `^-`  not currently used

### SSH Configuration

Assuming you want to use SSH with the default `orangepi` user through a local network where you (the client) and the OrangePi (the host) are connected. As `orangepi`, create the folder `~/.ssh` if not already existing, with a file `authorized_keys` inside the folder. We also need to give the right permissions 

```
$ mkldir ~/.ssh && chmod 700 ~/.ssh
$ cd ~/.ssh
$ touch authorized_keys && chmod 600 authorized_keys
```

Run `sudo systemctl status ssh`. If the service is not enabled, just type:

```
$ sudo systemctl enable ssh
$ sudo systemctl restart ssh
```

Open the SSH configuration file `/etc/ssh/sshd_config` with your editor of choice. You may need to modify the SSH port depending on your network firewall:

```
#Port 22
```

22 is the default. Commented statement are the default ones. For now, ONLY change the port if needed. To apply the modification, first check for errors with `sudo sshd -t`.

Then run `sudo systemctl reload ssh` to load the new configuration.

If you check the status of the SSH service, you will see it listens on the new port. You can confirm by checking the active sockets the user have access to by running `ss -unplat`.

On a client machine, generate a key:

```
> ssh-keygen -t rsa -b 2048
```

RSA is well supported by the OrangePi. Now a small prompt will ask you where to put your pair of keys. Usually, best practice is to have your keys in one of these directories:
- `/home/<username>/.ssh`    (most Unix systems like Linux distros, BSD distros and MAC)
- `C:\Users\<username>\.ssh` (Windows)

Feel free to give a nice name to the keys. After, you have to send the public key to the host, that is to say on OrangePi for the `orangepi` user. Find out the local IP address of the OrangePi:

```
$ ip a
```

Try to connect with the password method first, to see if the setup works:

```
> ssh orangepi@<ip_addr> -p <port>
```

You should see the following output if everything is ok:

```
The authenticity of host '[<ip_addr>]:1040 ([<ip_addr>]:<port>)' can't be established.
ED25519 key fingerprint is <...>.
This key is not known by any other names.
Are you sure you want to continue connecting (yes/no/[fingerprint])?
```

Type `yes`. Now you need to type the `orangepi` user password.

```
Warning: Permanently added '[<ip_addr>]:<port>' (ED25519) to the list of known hosts.
orangepi@<ip_addr>'s password:
```

If it worked, you can see the welcom message from the OrangePi:

```
  ___  ____ ___   _____             _____
 / _ \|  _ \_ _| |__  /___ _ __ ___|___ /
| | | | |_) | |    / // _ \ '__/ _ \ |_ \
| |_| |  __/| |   / /|  __/ | | (_) |__) |
 \___/|_|  |___| /____\___|_|  \___/____/

Welcome to Orange Pi 1.0.0 Jammy with Linux 5.4.125

System load:   26%              Up time:       1:08     Local users:   3
Memory usage:  15% of 3.81G     IP:            <ip_addr>
CPU temp:      48°C             Usage of /:    18% of 29G

Last login: Wed Jun 18 16:07:01 2025
orangepi@orangepizero3:~$
```

Logout from the OrangePi. On the client, **send the public key**, NOT THE PRIVATE. On UNIX systems you can do this in a single easy command:

```
> ssh-copy-id -i /path/to/key/<key_name>.pub "-oPort=<port>" orangepi@<ip_addr>
```

On Windows, it's a bit longer. Connect in SSH with password **then copy the content of your PUBLIC KEY** and paste inside the host at `~/.ssh/authorized_keys`. The whole key MUST FIT on a single line. You could also copy paste using SSH commands, but it didn't worked for me ...

Finally, inside the folder where your key are placed, creat a `config` file and write the following lines:

```
Host orangepi
	HostName <ip_addr>
	User orangepi
	Port <port>
	IdentityFile path/to/private/key/key_name
```

So now if you want to connect, you can just type `ssh orangepi` !

Last step is optional but better for security. On the host and inside `/etc/ssh/sshd_config` set `PasswordAuthentication no` and `PermitRootLogin no`, and make sure that you also have `PubkeyAuthentication yes`.

Run these commands for the last time:*

```
$ sudo sshd -t
$ sudo systemctl relaod ssh
```

Everything should be good now!

### OrangePi GPIO control

> Warning: you need to execute program as root if `/dev/gpiomem` does not exist.

There is a library built in C langage for OrangePi boards, called [WiringOP](https://github.com/zhaolei/WiringOP), which allows us to interact with the board GPIO pins easely, with a synthax similar to the one employed in the Arduino framework.

It should be already installed within the OrangePi OS. To check it, just type:

```
$ gpio -v
```

You may have an output similar to this:

```
gpio version: 2.46
Copyright (c) 2012-2018 Gordon Henderson
This is free software with ABSOLUTELY NO WARRANTY.
For details type: gpio -warranty

  * Device tree is enabled.
  *--> Orange Pi Zero 3
```

Then you can list your available GPIO pins:

```
$ gpio readall
```

And see the output:

```
 +------+-----+----------+--------+---+   H616   +---+--------+----------+-----+------+
 | GPIO | wPi |   Name   |  Mode  | V | Physical | V |  Mode  | Name     | wPi | GPIO |
 +------+-----+----------+--------+---+----++----+---+--------+----------+-----+------+
 |      |     |     3.3V |        |   |  1 || 2  |   |        | 5V       |     |      |
 |  229 |   0 |    SDA.3 |    OFF | 0 |  3 || 4  |   |        | 5V       |     |      |
 |  228 |   1 |    SCL.3 |    OFF | 0 |  5 || 6  |   |        | GND      |     |      |
 |   73 |   2 |      PC9 |    OFF | 0 |  7 || 8  | 0 | OFF    | TXD.5    | 3   | 226  |
 |      |     |      GND |        |   |  9 || 10 | 0 | OFF    | RXD.5    | 4   | 227  |
 |   70 |   5 |      PC6 |   ALT5 | 0 | 11 || 12 | 0 | OFF    | PC11     | 6   | 75   |
 |   69 |   7 |      PC5 |   ALT5 | 0 | 13 || 14 |   |        | GND      |     |      |
 |   72 |   8 |      PC8 |    OFF | 0 | 15 || 16 | 0 | OFF    | PC15     | 9   | 79   |
 |      |     |     3.3V |        |   | 17 || 18 | 0 | OFF    | PC14     | 10  | 78   |
 |  231 |  11 |   MOSI.1 |    OFF | 0 | 19 || 20 |   |        | GND      |     |      |
 |  232 |  12 |   MISO.1 |    OFF | 0 | 21 || 22 | 0 | OFF    | PC7      | 13  | 71   |
 |  230 |  14 |   SCLK.1 |    OFF | 0 | 23 || 24 | 0 | ALT3   | CE.1     | 15  | 233  |
 |      |     |      GND |        |   | 25 || 26 | 0 | OFF    | PC10     | 16  | 74   |
 |   65 |  17 |      PC1 |    OFF | 0 | 27 || 28 |   |        |          |     |      |
 |  272 |  18 |     PI16 |    OFF | 0 | 29 || 30 |   |        |          |     |      |
 |  262 |  19 |      PI6 |    OFF | 0 | 31 || 32 |   |        |          |     |      |
 |  234 |  20 |     PH10 |   ALT3 | 0 | 33 || 34 |   |        |          |     |      |
 +------+-----+----------+--------+---+----++----+---+--------+----------+-----+------+
 | GPIO | wPi |   Name   |  Mode  | V | Physical | V |  Mode  | Name     | wPi | GPIO |
 +------+-----+----------+--------+---+   H616   +---+--------+----------+-----+------+
```

> Note: WiringOP is a modified WiringPi for OrangePi. For code documentation please visit the [WiringOP GitHub repository](https://github.com/WiringPi/WiringPi).

Install the `libgpiod` library.

```
$ sudo apt-get update
$ sudo apt-get install libgpiod-dev gpiod
```

Check your GPIO pins:

```
$ gpiodetect
```

If you have:

```
gpiodetect: unable to access GPIO chips: Permission denied
```

Then you need to add new permissions to use the GPIO pins. We create a new group `gpio` for GPIO usage. Add the `orangepi` user to this group.

```
$ sudo groupadd gpio
$ sudo usermod -aG gpio orangepi
$ newgrp gpio
```

Create a udev rule file:

```
$ sudo nano /etc/udev/rules.d/99-gpio.rules
```

Add this content to set group `gpio` and permissions `660` on `/dev/gpiochip*` devices:

```bash
KERNEL=="gpiochip*", SUBSYSTEM=="gpio", MODE="0660", GROUP="gpio"
```

Apply rules:

```
$ sudo udevadm control --reload-rules
$ sudo udevadm trigger
```

Check permissions again:

```
$ ls -l /dev/gpiochip*
```

You should see a similar output:

```
crw-rw---- 1 root gpio 254, 0 Jun 25 13:00 /dev/gpiochip0
crw-rw---- 1 root gpio 254, 1 Jun 25 13:00 /dev/gpiochip1
```

### PWM Setup

If, for example, you want to use the PWM pins 3 and 4 for your project, you need to export them as `root` each time you boot the Orange Pi, using the following commands:

>[!NOTE]
> Make sure the `orangepi` user is member of the `gpio` group.

```
# echo 3 > /sys/class/pwm/pwmchip0/export
# chown :gpio /sys/class/pwm/pwmchip0/pwm3/*
# chmod g+rw /sys/class/pwm/pwmchip0/pwm3/*

# echo 4 > /sys/class/pwm/pwmchip0/export
# chown :gpio /sys/class/pwm/pwmchip0/pwm4/*
# chmod g+rw /sys/class/pwm/pwmchip0/pwm4/*
```

To avoid doing this manually, you can create a one-shot root Systemd service. Edit a file `/etc/systemd/system/pwm-fix-perms.service` and put the following lines:

```service
[Unit]
Description=Fix PWM permissions on boot
After=multi-user.target

[Service]
Type=oneshot
[Service]
Type=oneshot
ExecStart=/bin/sh -c "echo 3 > /sys/class/pwm/pwmchip0/export && echo 4 > /sys/class/pwm/pwmchip0/export && chown :gpio /sys/class/pwm/pwmchip0/pwm3/* && chmod g+rw /sys/class/pwm/pwmchip0/pwm3/* && chown :gpio /sys/class/pwm/pwmchip0/pwm4/* && chmod g+rw /sys/class/pwm/pwmchip0/pwm4/*"

[Install]
WantedBy=multi-user.target
```

Then reload Systemd and enable your service.

```
$ sudo systemctl daemon-reload
$ sudo systemctl enable pwm-fix-perms
$ sudo systemctl start pwm-fix-perms
```

By typing sudo systemctl status pwm-fix-perms, you should have an output similar to this:

```
○ pwm-fix-perms.service - Fix PWM permissions on boot
     Loaded: loaded (/etc/systemd/system/pwm-fix-perms.service; enabled; vendor preset: enabled)
     Active: inactive (dead) since Tue 2025-07-08 15:19:55 UTC; 7s ago
    Process: 7634 ExecStart=/bin/sh -c echo 3 > /sys/class/pwm/pwmchip0/export; echo 4 > /sys/class/pwm/pwmchip0/export; chown :gpio /sys/class/pwm/pwmchip0/p>
   Main PID: 7634 (code=exited, status=0/SUCCESS)
        CPU: 21ms

Jul 08 15:19:55 orangepizero3 systemd[1]: Starting Fix PWM permissions on boot...
Jul 08 15:19:55 orangepizero3 sh[7634]: /bin/sh: 1: echo: echo: I/O error
Jul 08 15:19:55 orangepizero3 sh[7634]: /bin/sh: 1: echo: echo: I/O error
Jul 08 15:19:55 orangepizero3 systemd[1]: pwm-fix-perms.service: Deactivated successfully.
Jul 08 15:19:55 orangepizero3 systemd[1]: Finished Fix PWM permissions on boot.
```

## Project Setup

### 🐋 Using Docker image

*TBA.*

### OpenCV Installation

Install OpenCV and dependencies

```
$ sudo apt update
$ sudo apt install -y libopencv-dev
```

Check installed version:

```
$ pkg-config --modversion opencv4
```

### Google Coral TPU Setup

#### Python Environment

> Note: Make a Python environment per type of model you want to run. For example, MobileNetV2 and YOLOvX require different library versions.

We need a Python environment in version 3.8 to match the libraries versions required to run inferences with the Google Coral TPU. To do so we first need to install Python 3.8. Problem is that it is quite old and we cannot access it the usual way. We are going to use *pyenv*.

Dependencies installation:

```
$ sudo apt-get install -y \
  make build-essential libssl-dev zlib1g-dev \
  libbz2-dev libreadline-dev libsqlite3-dev wget curl \
  libncursesw5-dev xz-utils tk-dev libxml2-dev \
  libxmlsec1-dev libffi-dev liblzma-dev git
```

*pyenv* installation:

```
$ curl https://pyenv.run | bash
```

Add *pyenv* to the load path by adding the following lines to `~/.bashrc` or `~/.zshrc`:

```bash
export PATH="$HOME/.pyenv/bin:$PATH"
eval "$(pyenv init --path)"
eval "$(pyenv init -)"
eval "$(pyenv virtualenv-init -)"
```

Then restart your shell:

```
$ source ~/.bashrc
```

Install Python 3.8:

```
$ pyenv install 3.8.18
```

Create the virtual environment and activate it:

```
$ pyenv virtualenv 3.8.18 py38
$ pyenv activate py38
```

To check if everything is right:

```
$ which python
$ python --version
```

In case you need to deactivate the Python environment, you can type ` pyenv deactivate`.

#### Packages for Google Coral TPU

We are going to use a mixture of packages from both Debian-based system packet manager and `pip`.

```
$ echo "deb https://packages.cloud.google.com/apt coral-edgetpu-stable main" | sudo tee /etc/apt/sources.list.d/coral-edgetpu.list
$ curl https://packages.cloud.google.com/apt/doc/apt-key.gpg | sudo apt-key add -
$ sudo apt-get update
```

Ignore the warning concerning legacy `trusted.gpg` keyring.
Type the following command to install our first package:

```
$ sudo apt-get install libedgetpu1-std
```

> Note that the Edge TPU Compiler is not supported on ARM64 systems. So we do not install it, but it is still more reliable to pre-compile a model on a stronger desktop before giving it to the OrangePi.

Then for the second package, we need to use our `py38` Python environment on Python 3.8. So we will use `pip` instead.

```
$ pip install --extra-index-url https://google-coral.github.io/py-repo/ pycoral
```

In `~/.bashrc` add the path to `LD_LIBRARY_PATH`:

```bash
export LD_LIBRARY_PATH=/usr/lib/x86_64-linux-gnu:$LD_LIBRARY_PATH
```

It should help to support TensorFlow Lite. Then run `source ~/.bashrc`.

#### USB bind to TPU

Check if `orangepi` user is member of the `plugdev` group:

```
$ groups
```

If not, add `orangepi` user to `plugdev` then restart the shell:

```
$ sudo usermod -aG plugdev $USER
$ newgrp plugdev
```

Now just run `lsusb`. You should see the Google Coral TPU, if connected to the board:

```
Bus 002 Device 003: ID 18d1:9302 Google Inc.
```

If you do not see `Google Inc` for now do not worry. You may see `Global Unichip Corp` instead. In that case please read carefully this section without executing anything yet. Otherwise, type the following command:

```
$ udevadm info --query=all --name=/dev/bus/usb/002/003
```

Where `002` and `003` refer to the previous output obtained with `lsusb`. Look for the following line:

```
ID_VENDOR_ID=18d1
ID_MODEL_ID=9302
```

Create a Udev Rule with a new file for the Coral USB:

```
$ sudo nano /etc/udev/rules.d/99-coral-usb.rules
```

And add this single line:

```bash
SUBSYSTEM=="usb", ATTR{idVendor}=="18d1", ATTR{idProduct}=="9302", MODE="0666", GROUP="plugdev"
```

Makew sure the numbers match your own findings. Apply the rules by running:

```
$ sudo udevadm control --reload-rules
$ sudo udevadm trigger
```

To see if it works, type:

```
$ ls -al /dev/bus/usb/002/003
```

If you have this output:

```
crw-rw-rw- 1 root plugdev 189, 130 Jun 20 08:04 /dev/bus/usb/002/003
```

It shows that it is working. Now unplug the Coral TPU, plug it back and run `lsusb`.

If you have the following output:

```
Bus 001 Device 006: ID 1a6e:089a Global Unichip Corp.
```

Make another Udev Rule. It is a known other device refering to the Google Coral.

```
$ sudo nano /etc/udev/rules.d/99-unichip-usb.rules
```

And add this single line:

```bash
SUBSYSTEM=="usb", ATTR{idVendor}=="1a6e", ATTR{idProduct}=="089a", MODE="0666", GROUP="plugdev"
```
Makew sure the numbers match your own findings.

```
$ sudo udevadm control --reload-rules
$ sudo udevadm trigger
```

To see if it works, type:

```
$ ls -al /dev/bus/usb/001/006
```

Plug and unplug, run `lsusb`.

```
Bus 001 Device 007: ID 1a6e:089a Global Unichip Corp.
```

Then if you have the following access rights:

```
crw-rw-rw- 1 root plugdev 189, 6 Jun 20 08:15 /dev/bus/usb/001/007
```

It should be good. You see that the access rights have been kept.

Run this `test_lib.py` file.

```py
import ctypes

try:
    ctypes.CDLL('libedgetpu.so.1')
    print("libedgetpu.so.1 loaded successfully")
except Exception as e:
    print("Failed to load libedgetpu.so.1:", e)
```

You should have the following output:

```
libedgetpu.so.1 loaded successfully
```

Now everything should be ready.

#### Run Pre-Compiled Models

> Note: Make sure you are inside a different Python environment per type of model you want to run. For example, MobileNetV2 and YOLOvX require different library versions.

Now it is time to download and test an example.

##### MobileNetV2

Download the ressources.

```
$ wget https://github.com/google-coral/test_data/raw/master/mobilenet_v2_1.0_224_inat_bird_quant_edgetpu.tflite
$ wget https://github.com/google-coral/test_data/raw/master/inat_bird_labels.txt
$ wget https://github.com/google-coral/test_data/raw/master/parrot.jpg
```

Make this `inference_model.py` example.

```py
import gc
import time
from PIL import Image
from pycoral.utils.edgetpu import make_interpreter
from pycoral.adapters import common, classify

def run_inference_tpu(model_path, image_path, labels_path, top_k=1, count=5):
    print("Initializing TF Lite interpreter (Edge TPU)...")
    interpreter = make_interpreter(model_path)
    interpreter.allocate_tensors()
    print("INFO: Initialized TensorFlow Lite runtime.")

    size = common.input_size(interpreter)
    image = Image.open(image_path).convert('RGB').resize(size)
    common.set_input(interpreter, image)

    print("----INFERENCE TIME----")
    print("Note: The first inference on Edge TPU is slow because it includes loading the model into Edge TPU memory.")
    for _ in range(count):
        start = time.perf_counter()
        interpreter.invoke()
        elapsed_ms = (time.perf_counter() - start) * 1000
        print(f"{elapsed_ms:.1f}ms")

    classes = classify.get_classes(interpreter, top_k=top_k)

    with open(labels_path) as f:
        labels = [line.strip() for line in f.readlines()]

    print("-------RESULTS--------")
    for c in classes:
        label = labels[c.id] if c.id < len(labels) else "Unknown"
        print(f"{label}: {c.score:.5f}")

    del interpreter
    gc.collect()

if __name__ == "__main__":
    MODEL_TPU = "mobilenet_v2_1.0_224_inat_bird_quant_edgetpu.tflite"
    IMAGE_PATH = "parrot.jpg"
    LABELS_PATH = "inat_bird_labels.txt"
    TOP_K = 1
    COUNT = 5

    print("=== TPU Model Inference ===")
    run_inference_tpu(MODEL_TPU, IMAGE_PATH, LABELS_PATH, TOP_K, COUNT)
```

Try to execute it inside a subprocess with this `example_model.py`.

```py
import subprocess

subprocess.run(["python", "inference_model.py"])
```

> We manually delete the interpreter and call the garbage collector to flush the TPU memory. Otherwise we might have issue running the program multiple times.

Program output:

```
=== TPU Model Inference ===
Initializing TF Lite interpreter (Edge TPU)...
INFO: Initialized TensorFlow Lite runtime.
----INFERENCE TIME----
Note: The first inference on Edge TPU is slow because it includes loading the model into Edge TPU memory.
112.4ms
14.4ms
14.3ms
14.4ms
14.2ms
-------RESULTS--------
Ara macao (Scarlet Macaw): 0.75391
```

##### YOLOv8

Install the package:

```
$ pip install ultralytics
```

Make a Python script `example_model`.

```py
from ultralytics import YOLO

# Load a model
model = YOLO("yolov8n_full_integer_quant_edgetpu.tflite")

images = ["im1.jpg", "im2.jpg", "im3.jpg"]

# Run Prediction
for i, img in enumerates(images):
    r = model.predict(img, imgsz=224, conf=0.5, device="tpu:0")
    r[0].save(f"result{i}.jpg")
```

You need to have a quantified model and images to run the prediction on, and that is it! Just run the program.
