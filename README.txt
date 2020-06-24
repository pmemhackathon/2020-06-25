This is the workshop repo.  If you're reading this through a webhackathon
window or a workshop SSH session, then you're probably looking at your
own private copy of the repo.

This repo contains the slides and examples used during the virtual
PMDK Summit workshop on June 25, 2020.

It is provided permanently for reference on GitHub:

	https://github.com/pmemhackathon/2020-06-25

The PDF files at the top level contain slides:

	slides.pdf         -- essential hackathon background
	slides-llpl.pdf    -- llpl background
	pmdk-overview.pdf  -- overview of Persistent Memory Development Kit
	libpmemobj.pdf     -- overview of libpmemobj (C API)
	libpmemobj-cpp.pdf -- overview of libpmemobj (C++ API)

The examples are in examples/A, exmamples/B, etc.  Each example is meant
to show a persistent memory programming concept, like transactions,
support for a specific language, and API, etc.	The idea is to use
the first few examples (A through C) to gain some essential background
knowledge, then pick examples that are interesting to you.  Use them as a
starting point, or a reference as you develop your own persistent memory
aware programs.  There's a README.txt file in each example directory
that explains what the example is all about.

For more information, contact andy.rudoff@intel.com or post your question
to the "pmem" google group on groups.google.com.


The containers used during the hackathons have all the necessary libraries
installed in them.  You can access the container directly on DockerHub
in the "pmemhackathon" area.  Here are the steps taken to install the
libraries on a Linux machine.  These steps assume you have a machine
with persistent memory installed and a recent Linux distro that supports
persistent memory.

We don't show the ipmctl and ndctl commands used to configure the
persistent memory.  These examples assume persistent memory is available
as a DAX mounted filesystem at "/pmem" on your system.  The following link
provides more details about the steps to configure persistent memory:

	https://docs.pmem.io/getting-started-guide

#
# To clone this repo, use something like the command below.
# (Examples assume cloning in your home directory, hence the "cd")
#

cd
git clone https://github.com/pmemhackathon/2020-06-25

#
# Some of the examples use PMDK.  Many distros include PMDK, but
# it takes some time for the latest version to flow intro the distros
# so here are the steps to build the latest source and install it in
# the usual locations on Fedora 30.
#

cd
# ubuntu: sudo apt-get install autoconf pkg-config libndctl-dev libdaxctl-dev
sudo dnf install autoconf pkg-config ndctl-devel daxctl-devel
git clone https://github.com/pmem/pmdk
cd pmdk
make
sudo make install prefix=/usr

#
# Also install the C++ bindings for libpmemobj for the C++ example.
#
cd
# ubuntu: sudo apt-get install cmake
sudo dnf install cmake
git clone https://github.com/pmem/libpmemobj-cpp
cd libpmemobj-cpp
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX:PATH=/usr ..
make
sudo make install

#
# Also install libpmemkv for the key-value example.
#

cd
git clone https://github.com/pmem/pmemkv
cd pmemkv
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/usr
make
sudo make install
