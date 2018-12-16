
FROM vbatts/slackware
LABEL maintainer="Dave Goehrig <dave@dloh.org>"

RUN echo "Y" | slackpkg install yasm
RUN echo "Y" | slackpkg install libffi
RUN echo "Y" | slackpkg install guile
RUN echo "Y" | slackpkg install gc
RUN echo "Y" | slackpkg install make
RUN echo "Y" | slackpkg install git
RUN echo "Y" | slackpkg install cyrus-sasl
RUN echo "Y" | slackpkg install curl
RUN echo "Y" | slackpkg install vim
RUN echo "Y" | slackpkg install perl
RUN echo "Y" | slackpkg install python
RUN mv /usr/bin/vi /usr/bin/vi.old
RUN ln -s /usr/bin/vim /usr/bin/vi
