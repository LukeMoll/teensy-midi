#!/bin/bash -
set -e
python3 -m virtualenv venv
source venv/bin/activate
pip install -r pip-requirements.txt