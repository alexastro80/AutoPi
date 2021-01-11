
python3 -c "import sys, pkgutil; sys.exit(0 if pkgutil.find_loader('"$1"') else 1)"