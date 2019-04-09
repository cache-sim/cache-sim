## Analysis of different cache parameters on hit ratio

We have analysed performance of matrix multiplication programs with respect to hit ratio in our one level cache simulator.

For analysis, we considered the following three variants of matrix multiplication (the programs used can be found [here](./matrix)):
- P1: DGEMM (Double precision General Matrix Multiply)
- P2: DGEMM with row-major accesses only
- P3: Tiled DGEMM 

We considerd matrices of size 512 X 512 for the above programs. Let A and B be the two matrices to multiply and let C = A* B.

The results presented below are after running around 250 simulations in our simulator.

Analysis on hit-ratio is done on the following:
 - [Impact of Block Size](#impact-of-block-size)
 - [Impact of Number of Sets](#impact-of-number-of-sets)
 - [Impact of Set Associativity](#impact-of-set-associativity)
 - [Impact of Tile Size](#impact-of-tile-size)

### Impact of Block Size

Here we have varied block size of cache over values 8, 16, 32, 64 and 128 bytes. The set associativity and number of sets is kept constant at 4 and 64 respectively. 

**NOTE : Since set associativity and number of sets is constant and block size increases, the overall size of cache also increases.**

Following are the results obtained:

![P1: Varying block size](./images/varying-blocksize-P1.png)

##### Analysis:

 All the cache replacement policies do poorly when block size is 8. For a cache with block size 8, just one element from the matrix can is stored on a cache line. In P1, once some element is accessed in A and B, it won't be accessed again soon (accessed again only after completion of inner k loop). So the hit ratio is less. This also explains the drastic increase in hit ratio for all replacement policies when block size is increased from 8 to 16 bytes.

A cache with block size 16 can accomadate 512 double values. This is same as the number of elements in a row of A. Thus, LRU and SRRIP saturate after block size 16 since an entire row of A can be stored when the cache has block sizes greater than 16.

In P1, A is accessed in row major wise and B is accessed column major wise. Hence it is favourable to remove elements in B rather than in A during cache eviction. During eviction, LRU and SRRIP mostly remove values pertaining to B over A. This is not true for NRU and LFU, as they might remove values of A even when there are values of B in the same set which will definitely not be used. Thus LRU and SRRIP perfrom vastly better than NRU and LFU. Also as block size increases, per cache line number of hits for NRU and LFU increases, leading to steady increase in hit ratio as block size increases. The same trend is observed in PLRU for the same reasons.

PLRU has an anomoly: sudden decrease in hit ratio for block size 64. We are not sure of the exact reason for this. It might be due to the undeterministic nature of bit-PLRU algorithm.

Due to column major access of B, for all block sizes we analysed, B will always suffer a lot of cold start misses. Hence we observe that all hit ratios are capped at 95% for all replacement policies.

![P2: Varying block size](./images/varying-blocksize-P2.png)

##### Analysis:

In P2, inside the k for loop, one value of A is accessed and an entire row of B and C is accessed. To accomadate two rows of B and C inside the cache, the cache needs to atleast have a block size of 32. Hence till 32, LRU, SRRIP and PLRU have a drastic increase and NRU and LFU have a steady increase. And, hit ratio almost saturates after block size 32 in SRRIP, LRU and PLRU for the same reason.

NRU and LFU perform poorer from SRRIP and LRU due to poor choice of cache line for eviction. Thus hit ratio is below 90 for block size 32 and 64. But for block size 128, the cache is big enough to hold so many values of the matrix that irrespective of the replacement policy, the hit ratio will be high for all policies. Hence we see a sudden increase in hit ratio for NRU and LFU (also PLRU) with block size 128.

![P3: Varying block size](./images/varying-blocksize-P3.png)

##### Analysis:

We have analysed P3 with tile size 4. P3 has high temporal locality but is also affected by thrashing in relatively small caches. The hit ratio for P3 is majorly defined by how much these two factors contribute.

For all replacement policies, there is a steady increase of hit ratio till block size 64 since thrashing keeps reducing as the cache size increases. LRU and SRRIP have a drastic increase in hit ratio for block size 128. With this block size, a single cache line can hold 16 double values. So a single set (4 way set associative) can hold upto 4 consecutive tiles of a matrix. With replacement policies like LRU and SRRIP, this results in drastic increase in hit ratio.

#### Overall Analysis:

- P2 has more temporal locality than P1. Hence, the hit ratio for all replacement policies is better for P2 than P1. 
- P3 has more temporal locality than P1. Hence, the hit ratio for all replacement policies with block size  less than 16 is better for P3 than P2. But this trend stops after block size 16. This could be because of thrashing in P3, which negates it's high temporal locality and hence a lesser hit ratio.

### Impact of Number of Sets

Here we have varied number of sets in cache over values 16, 32, 64, 128 and 256. The block size and set associativity is kept constant at 32 bytes and 4 respectively.

Following are the results obtained:

![P1: Varying number of sets](./images/varying-number-of-sets-P1.png)

##### Analysis:

In both LRU and SRRIP, when the number of sets becomes 32, the total number of double variables that the cache can hold is 512, which is the size of one row of the matrix and since the values stored depends on the number of accesses, matrix A's elements are stored in the cache leading to high number of hits. In both LRU and SRRIP replacement algorithms, as mentioned above, the hit rate has a cap on the performance due to the unnovoidable cold start misses during the column major accesses of the elements in the B matrix. Hence, with the increase in number of sets (leading to an increase in block size), the increase in hit ratio is very minimal.

In NRU there is a constant increase in the hit ratio simply due to the fact that the cache size increasese with increase in number of sets. This enables the cache to hold onto the values of a row in matrix A for longer, hence giving a higher hit ratio. The algorithm still evicts the wrong elements (i.e. the elements in the beginning of the cache set), but the effect is reduced gradually due to the increasing size of the cache.



![P2: Varying number of sets](./images/varying-number-of-sets-P2.png)

##### Analysis:



![P3: Varying number of sets](./images/varying-number-of-sets-P3.png)


### Impact of Set Associativity

Here we have varied set associativity in cache over values 2, 4, 8, 16 and 32. The block size and number of sets is kept constant at 32 bytes and 64 respectively.

Following are the results obtained:

![P1: Varying set associativity](./images/varying-set-associativity-P1.png)

![P2: Varying set associativity](./images/varying-set-associativity-P2.png)

![P3: Varying set associativity](./images/varying-set-associativity-P3.png)


### Impact of Tile Size

Here we have varied tile size in P3 over values 4, 8 and 16 (side of tile). The block size, set associativity and number of sets is kept constant at 32 bytes, 16 and 64 respectively.

Following are the results obtained:

![P3: Varying tile size](./images/varying-tile-size-P3.png)

#### Relevant links
1. [Link](https://drive.google.com/drive/folders/1JsXtNrd9Myawke7c2M9BEU8nSjW3ATSR) to traces of the programs.
2. [Link](https://docs.google.com/spreadsheets/d/1dfC8pnfuAcPPloEfE03zAsp8cmzvUKPmlbnhgLS_E7s/edit?usp=sharing) to google sheets with our results.