#!/bin/bash
#
# This file was automatically generated by s2e-env at 2020-08-13 13:42:57.408655
#
# This bootstrap script is used to control the execution of the target program
# in an S2E guest VM.
#
# When you run launch-s2e.sh, the guest VM calls s2eget to fetch and execute
# this bootstrap script. This bootstrap script and the S2E config file
# determine how the target program is analyzed.
#

set -x



mkdir -p guest-tools32
TARGET_TOOLS32_ROOT=guest-tools32




TARGET_TOOLS_ROOT=${TARGET_TOOLS32_ROOT}



# To save the hassle of rebuilding guest images every time you update S2E's guest tools,
# the first thing that we do is get the latest versions of the guest tools.
function update_common_tools {
    local OUR_S2EGET

    OUR_S2EGET=${S2EGET}

    # First, download the common tools


    for TOOL in ${COMMON_TOOLS}; do
        ${OUR_S2EGET} ${TARGET_TOOLS_ROOT}/${TOOL}
        chmod +x ${TOOL}
    done
}

function update_target_tools {
    for TOOL in $(target_tools); do
        ${S2EGET} ${TOOL} ${TOOL}
        chmod +x ${TOOL}
    done
}

function prepare_target {
    # Make sure that the target is executable
    chmod +x "$1"
}



# This function executes the target program given in arguments.
#
# There are two versions of this function:
#    - without seed support
#    - with seed support (-s argument when creating projects with s2e_env)
function execute {
    local TARGET
    local SEED_FILE
    local SYMB_FILE

    TARGET=$1

    prepare_target "${TARGET}"



    execute_target "${TARGET}" "${SYMB_FILE}"

}

###############################################################################
# This section contains target-specific code

function make_seeds_symbolic {
    echo 1
}

# This function executes the target program.
# You can customize it if your program needs special invocation,
# custom symbolic arguments, etc.
function execute_target {
    local TARGET
    local SYMB_FILE

    TARGET="$1"
    SYMB_FILE="$2"


    S2E_SO="${TARGET_TOOLS32_ROOT}/s2e.so"



    ./${TARGET}  > /dev/null 2> /dev/null

}

# Nothing more to initialize on Linux
function target_init {
    # Start the LinuxMonitor kernel module
    sudo modprobe s2e
}

# Returns Linux-specific tools
function target_tools {

    echo "${TARGET_TOOLS32_ROOT}/s2e.so"

}

S2ECMD=./s2ecmd
S2EGET=./s2eget
S2EPUT=./s2eput
COMMON_TOOLS="s2ecmd s2eget s2eput"

###############################################################################


update_common_tools
update_target_tools



# Don't print crashes in the syslog. This prevents unnecessary forking in the
# kernel
sudo sysctl -w debug.exception-trace=0

# Prevent core dumps from being created. This prevents unnecessary forking in
# the kernel
ulimit -c 0

# Ensure that /tmp is mounted in memory (if you built the image using s2e-env
# then this should already be the case. But better to be safe than sorry!)
if ! mount | grep "/tmp type tmpfs"; then
    sudo mount -t tmpfs -osize=10m tmpfs /tmp
fi

# Need to disable swap, otherwise there will be forced concretization if the
# system swaps out symbolic data to disk.
sudo swapoff -a



target_init

# Download the target file to analyze


${S2EGET} "demo.cc"                #use [S2EGET] to pass the script in to the image
cp demo.cc ns-3-dev/scratch        #copy the script to scratch folder
cd ns-3-dev                        #run the script
./waf --run demo

##### NO TARGET HAS BEEN SPECIFIED DURING PROJECT CREATION #####
##### Please fetch and execute the target files manually   #####
