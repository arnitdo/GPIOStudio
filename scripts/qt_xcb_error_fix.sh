echo "This script is supposed to fix the \"No platform xcb was found\" error"
echo ""
echo "Run this script in the root GPIOStudio directory"
echo ""
echo "Please Press Y or ENTER when prompted"
echo ""
echo "The script will now install the basic dependencies"
sudo apt install libxcb-xinerama0
sudo apt install libxkb*
echo ""
echo "The problem now should have been fixed, if not, please create an issue here"
echo ""
echo "https://github.com/arnitdo/GPIOStudio/issues/"