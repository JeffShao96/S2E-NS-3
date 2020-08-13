# S2E-NS-3 Build Instructions


## Build s2e-env
We highly recommand to build with s2e-env. However, you can manually build S2E as well. 

### Install the packages for s2e-env
  sudo apt-get update
  sudo apt-get install git gcc python3 python3-dev python3-venv

### Clone the source code
`git clone https://github.com/s2e/s2e-env.git`
`cd s2e-env`

### Create an virtual environment to build S2E
`python3 -m venv venv`
`. venv/bin/activate`

### Do the following steps in the virtual environment
### If you want to quit the virtual environment, use command `deactivate`

### Upgrade the python packages
`pip install --upgrade pip`

### Install the s2e-env`
`pip install .`

### Note: if your pip version is earlier than v19, use the following command:
`pip install --process-dependency-links .`


## Build S2E
### create a directory
`mkdir $S2EDIR`
`cd $S2EDIR`
