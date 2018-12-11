
FROM vbatts/slackware
LABEL maintainer="Dave Goehrig <dave@dloh.org>"

RUN slackpkg install yasm
RUN slackpkg install libffi
RUN slackpkg install guile
RUN slackpkg install gc
RUN slackpkg install make
RUN slackpkg install git
RUN slackpkg install cyrus-sasl
RUN slackpkg install curl
RUN slackpkg install vim
RUN slackpkg install perl
RUN slackpkg install python
RUN mv /usr/bin/vi /usr/bin/vi.old
RUN ln -s /usr/bin/vim /usr/bin/vi
