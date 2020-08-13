# S2E-NS-3 Build Instructions


## Build S2E with s2e-env
We highly recommand to build S2E with s2e-env. However, you can manually build S2E as well. 

#### Install the packages for s2e-env
    sudo apt-get update
    sudo apt-get install git gcc python3 python3-dev python3-venv

#### Clone the source code for s2e-env
    git clone https://github.com/s2e/s2e-env.git

#### Create an virtual environment to build S2E
    cd s2e-env
    python3 -m venv venv
    . venv/bin/activate
**You should do the following steps in the virtual environment**

#### If you want to quit the virtual environment, use the following command:
    deactivate

#### Upgrade the python packages
    pip install --upgrade pip

#### Install the s2e-env
    pip install .
**Note: if your pip version is earlier than v19, use the following command:**
    pip install --process-dependency-links .

#### Download the source code for S2E
    mkdir $S2EDIR
    cd $S2EDIR
    s2e init

#### build the source code
    s2e build
The process takes about 60 mins.
