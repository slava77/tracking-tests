TrackingNtuple

how-to sparse-checkout only this directory

git init TrackingTests
cd TrackingTests/
git remote add -f origin git@github.com:cerati/tracking-tests
git config core.sparseCheckout true
echo "TrackingNtuple/" >> .git/info/sparse-checkout
git pull origin master
cd TrackingNtuple/
