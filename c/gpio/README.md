This sample code demonstrates writing to Digital pin 4, 5 and 6 of Galileo

Follow the below instruction for building this code:

# bitbake image-full -c populate_sdk
Make sure to source poky/oe-init-build-env yocto_build

The output of the build process is a script that installs the toolchain on another system:
clanton-full-eglibc-x86_64-i586-toolchain-1.4.1.sh
The script is located in ./tmp/deploy/sdk 

Note: The script may change your environment significantly, thus breaking other, non-Yocto 
tools you might be using (including anything which uses Python). To mitigate, Intel 
recommends that you open one terminal session to source the Yocto environment and 
run make, and run all your other commands in other terminal sessions. 

When you are ready to compile your application, first run the source command below 
to define default values for CC, CONFIGURE_FLAGS, and other environment variables, 
then you can compile: 
# source /opt/poky/1.4.2/environment-setup-x86_32-poky-lin
#make
