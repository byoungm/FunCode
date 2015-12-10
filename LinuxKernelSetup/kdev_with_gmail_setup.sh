#!/bin/sh

echo "What is your Name?"
read NAME
echo "What is your Gmail User Name?"
read GMAIL_USER
echo "What is your Password?"
read GMAIL_PASSWORD


# Save MuttRC File
MUTTRC_FILE=~/.muttrc

cat > $MUTTRC_FILE <<- EOM
# A basic .muttrc for use with Gmail
# Change the following six lines to match your Gmail account details
set imap_user = "$GMAIL_USER@gmail.com"
set imap_pass = "$GMAIL_PASSWORD"
set smtp_url = "smtp://$GMAIL_USER@smtp.gmail.com:587"
set smtp_pass = "$GMAIL_PASSWORD"
set from = "$GMAIL_USER@gmail.com"
set realname="$NAME"
# Change the following line to a different editor you prefer.
set editor = "nano"
# Basic config, you can leave this as is
set ssl_starttls = yes
set ssl_force_tls = yes
set header_cache="~/.mutt/cache/headers"
set message_cachedir="~/.mutt/cache/bodies"
set certificate_file="~/.mutt/certificates"
set folder = "imaps://imap.gmail.com:993"
set spoolfile = "+INBOX"
set imap_check_subscribed
set hostname = gmail.com
set mail_check = 120
set timeout = 300
set imap_keepalive = 300
set postponed = "+[GMail]/Drafts"
set record = "+[GMail]/Sent Mail"
set header_cache="~/.mutt/cache/headers"
set message_cachedir="~/.mutt/cache/bodies"
set certificate_file="~/.mutt/certificates"
set move = no
set include
set sort = 'threads'
set sort_aux = 'reverse-last-date-received'
set auto_tag = yes
ignore "Authentication-Results:"
ignore "DomainKey-Signature:"
ignore "DKIM-Signature:"
hdr_order Date From To Cc
alternative_order text/plain text/html *
auto_view text/html
bind editor <Tab> complete-query
bind editor ^T complete
bind editor <space> noop
# Gmail-style keyboard shortcuts
macro index,pager y "unset trash\n " "Gmail archive message"
macro index,pager d "set trash=\"imaps://imap.googlemail.com/[GMail]/Bin\"\n " "Gmail delete message"
macro index,pager gi "=INBOX" "Go to inbox"
macro index,pager ga "=[Gmail]/All Mail" "Go to all mail"
macro index,pager gs "=[Gmail]/Starred" "Go to starred messages"
macro index,pager gd "=[Gmail]/Drafts" "Go to drafts"
EOM


# Save git configuration
git config --global sendemail.smtpencryption tls
git config --global sendemail.smtpserver smtp.gmail.com
git config --global sendemail.smtpuser $GMAIL_USER.gmail.com
git config --global sendemail.smtpserverport 587
git config --global sendemail.smtppass $GMAIL_PASSWORD
git config --global user.name "$NAME"
git config --global user.email $GMAIL_USER@gmail.com
git config --global core.editor nano