FROM vbatts/slackware
LABEL maintainer="Dave Goehrig <dave@dloh.org>"
RUN slackpkg update
RUN echo "" | slackpkg install yasm
RUN echo "" | slackpkg install libffi
RUN echo "" | slackpkg install guile
RUN echo "" | slackpkg install gc
RUN echo "" | slackpkg install git
RUN echo "" | slackpkg install cyrus-sasl
RUN echo "" | slackpkg install curl
RUN echo "" | slackpkg install vim
RUN echo "" | slackpkg install perl
RUN echo "" | slackpkg install python
RUN echo "" | slackpkg install autoconf
RUN echo "" | slackpkg install automake
RUN echo "" | slackpkg install binutils
RUN echo "" | slackpkg install bison
RUN echo "" | slackpkg install doxygen
RUN echo "" | slackpkg install flex
RUN echo "" | slackpkg install gcc
RUN echo "" | slackpkg install gcc-g++
RUN echo "" | slackpkg install gettext-tools
RUN echo "" | slackpkg install kernel-headers
RUN echo "" | slackpkg install libtool
RUN echo "" | slackpkg install m4
RUN echo "" | slackpkg install make
RUN echo "" | slackpkg install pkg-config
RUN echo "" | slackpkg install libmpc
RUN echo "" | slackpkg install glibc
RUN mv /usr/bin/vi /usr/bin/vi.old
RUN ln -s /usr/bin/vim /usr/bin/vi
RUN mkdir /forth64
