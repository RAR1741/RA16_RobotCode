#!/bin/bash

apt-get install -y ubuntu-desktop
# Sets up Gazebo on a ubuntu machine.
#wget -O frcsim-installer.sh first.wpi.edu/FRC/roborio/release/frcsim-installer.sh && chmod a+x frcsim-installer.sh &&
yes | /vagrant/frcsim-installer.sh  INSTALL
