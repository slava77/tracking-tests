TrackingNtuple

how-to sparse-checkout only this directory

git init TrackingTests <br>
cd TrackingTests/ <br>
git remote add -f origin git@github.com:cerati/tracking-tests  <br>
git config core.sparseCheckout true  <br>
echo "TrackingNtuple/" >> .git/info/sparse-checkout  <br>
git pull origin master  <br>
cd TrackingNtuple/  <br>
