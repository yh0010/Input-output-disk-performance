echo "============Question 1=============="
gcc run.c -o run
echo "Write file with block size: 1024, block count: 1024000"
./run output -w 1024 1024000
echo ""
echo "Read the file created"
./run output -r 1024 1024000
echo ""
echo "Read half of the file"
./run output -r 1024 512000
echo ""
echo "============Question 2-5============="
gcc run2.c -o run2
echo "Write file with reasonable size"
./run output -w 2048 1024000
echo ""
echo "Read the file created"
./run2 output 1024
echo ""
echo "==============Caching================"
sudo sh -c "/usr/bin/echo 3 > /proc/sys/vm/drop_caches"
echo "Cache cleared, read the file with 1024 block size"
./run2 output 1024
echo ""
echo "Read the file with the same block size again"
./run2 output 1024
echo ""
echo "=============Small block============="
echo "Write a small file for quicker reading"
./run small -w 1024 100
echo ""
echo "Read the file file block size equals to 1"
gcc run5.c -o run5
./run5 small -r
echo ""
echo "Use lseek to get better system call speed"
./run5 small -s
echo ""
echo "============Extra Credit============="
echo "Read the file with dd command with bs=1024"
dd if=output of=o bs=1024
echo ""
echo "=============Question 6=============="
gcc -O3 fast.c -o fast
echo "See the optimized performance"
./fast output 