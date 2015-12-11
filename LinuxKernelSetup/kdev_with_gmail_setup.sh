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
# Folder hooks
folder-hook 'account.com.gmail.$GMAIL_USER' 'source ~/.mutt/account.com.gmail.$GMAIL_USER'

# Default account
source ~/.mutt/account.com.gmail.$GMAIL_USER

# Macros for switching accounts

macro index <F12> '<sync-mailbox><enter-command>source ~/.mutt/account.com.gmail.$GMAIL_USER<enter><change-folder>!<enter>'

# Set default text editor
set editor = "nano"

#-------- Basic Config ------------
set imap_check_subscribed
set mail_check = 120
set timeout = 300
set imap_keepalive = 300
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

EOM

if [ ! -d ~/.mutt ]; then
	mkdir ~/.mutt
fi

# Save MUTT Gmail Config
GMAIL_MUTT_FILE=~/.mutt/account.com.gmail.$GMAIL_USER

cat > $GMAIL_MUTT_FILE <<- EOM
set imap_user = "$GMAIL_USER@gmail.com"
set imap_pass = "$GMAIL_PASSWORD"
set smtp_url = "smtp://$GMAIL_USER@smtp.gmail.com:587/"
set smtp_pass = "$GMAIL_PASSWORD"
set from = "$GMAIL_USER@gmail.com"
set realname = "$NAME"
set folder = "imaps://imap.gmail.com:993"
set spoolfile = "+INBOX"
set postponed = "+[Gmail]/Drafts"
set header_cache = ~/.mutt/com.gmail.$GMAIL_USER/cache/headers
set message_cachedir = ~/.mutt/com.gmail.$GMAIL_USER/cache/bodies
set certificate_file = ~/.mutt/com.gmail.$GMAIL_USER/certificates
EOM

# Save git configuration
git config --global sendemail.smtpencryption tls
git config --global sendemail.smtpserver /usr/bin/msmtp
git config --global sendemail.smtpuser $GMAIL_USER.gmail.com
git config --global sendemail.smtpserverport 587
git config --global user.name "$NAME"
git config --global user.email $GMAIL_USER@gmail.com
git config --global core.editor nano

MSMTP_CONFIG_FILE=~/.msmtprc
cat > $MSMTP_CONFIG_FILE <<- EOM
# Setup MSMTP for git send-email
# Set default values for all following accounts.
defaults
tls on
tls_trust_file /etc/pki/tls/certs/ca-bundle.crt
logfile ~/.msmtp.log

# My email service
account gmail
host smtp.gmail.com
port 587
from $GMAIL_USER@gmail.com
auth on
user $GMAIL_USER@gmail.com

# Set a default account
account default : gmail

EOM

# Add download packages with DNF for fedora
sudo dnf install make gcc kernel-devel openssl-devel mutt nano git-email msmtp
