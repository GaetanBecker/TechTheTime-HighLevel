#!/bin/bash

green="$(tput bold ; tput setaf 2)"
reset="$(tput sgr0)"

cd ..

git submodule deinit --force --all

echo "===== Initializing submodules ====="

git submodule init
git submodule update --remote

echo "===== Finished ====="

cd build_tool

sudo chmod u+x build-hl.sh
sudo chmod u+x install-ros.sh

echo "Do you want proceed to the essentials installation (ros2-foxy, rosdep, colcon, build_essentials[curl, locales, etc...]) [Y/n]? : "

read answer

if [ "${answer,,}" = "y" ] || [ -z $answer ]; then
    ./install-ros.sh
    if [ $? -ne 0 ]; then
        echo "Failed to install"
        exit 1
    fi
    echo "===== Finished ====="
fi

cd ../ros_workspace

echo "Do you want to install all dependecies with rosdep (rosdep must be installed) [Y/n]? :  "

read answer

if [ "${answer,,}" = "y" ] || [ -z $answer ]; then
    rosdep install --from-paths src --ignore-src -r -y
    echo "===== Finished ====="
fi

echo "${green}Now you can run build-hl.sh. Run build-hl.sh -h first for help${reset}"

