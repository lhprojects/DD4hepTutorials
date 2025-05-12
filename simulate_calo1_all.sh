source /cvmfs/sw.hsf.org/key4hep/setup.sh
mkdir build install
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=../install -Wno-dev
make install -j 8
cd ..
k4_local_repo


for i in 1 2 3 4 5 6 7 8 9 
do
	cp simplecalo1/compact/simplecalo1.xml simplecalo1/compact/simplecalo1_$i.xml
	sed -i "s/1\*cm/${i}*cm/g" simplecalo1/compact/simplecalo1_$i.xml #Sen
	sed -i "s/9\*cm/$((10-i))*cm/g" simplecalo1/compact/simplecalo1_$i.xml #Abs
	ddsim --steeringFile simplecalo1/sc1SteeringFile.py --compactFile "simplecalo1/compact/simplecalo1_$i.xml" --outputFile simplecalo_$i.root --numberOfEvents 400
done

python plot_cell_energy_sum_resolutions.py


