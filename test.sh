# Test script for `soda`
# By Doug Sherk and Jimmy Yu
#
# Brute force the problem and grep out any deadlocks. This test assumes that the
# only things that can go wrong are the following:
# 1) The program deadlocks and uC++ detects it, or
# 2) The program fails catastrophically enough that the error output bypasses
#    the grep
# This will take a very long time to run. This is normal.
# There are 58320 tests.

num = 0
for a in {1..3..1}; do
  for b in {1..5..2}; do
    for c in {5..20..5}; do
      for d in {1..5..2}; do
        for e in {3..7..2}; do
          for f in {4..8..2}; do
            for g in {1..9..4}; do
              for h in {10..30..5}; do
                for i in {1..4..1}; do
                  echo "SodaCost $a" > test.config
                  echo "NumStudents $b" >> test.config
                  echo "MaxPurchases $c" >> test.config
                  echo "NumVendingMachines $d" >> test.config
                  echo "MaxStockPerFlavour $e" >> test.config
                  echo "MaxShippedPerFlavour $f" >> test.config
                  echo "TimeBetweenShipments $g" >> test.config
                  echo "ParentalDelay $h" >> test.config
                  echo "NumCouriers $i" >> test.config
                  num=`expr $num + 1`
                  echo "Test #$num/58320"
                  ./soda test.config | grep -i "deadlock"
                done
              done
            done
          done
        done
      done
    done
  done
done
