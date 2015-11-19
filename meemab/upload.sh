#!/bin/bash
rsync -avzhr --delete -e ssh ./webclient/ gigglingbits.com@ssh.gigglingbits.com:/www/meemab/
