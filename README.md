<h5 align = center> Copyright Cretu Mihnea Tudor 315CAa 2022 - 2023 </h5> 
<h5 align = center> 1st Homework for Data Structures and Algorithms Class, Year I - Faculty of Automation Control and Computer Science, Polytechnic University of Bucharest </h5>

<h1 align = center> Virtual Memory Allocator </h1>
<br>

<h2> 1) Introduction: </h2>

<p align = left> This program uses doubly linked lists to mimic a real memory allocator. The program uses a virtual arena with a given size in which the memory blocks and their respective miniblock lists will be allocated in. The arena itself and all of its components have been allocated dinamically in memory, thus the arena has to be deallocated from the memory before exiting the program. The information stored in the memory can be of any data type.</p>
<br>

<h2> 2) Installation: </h2>

<p>  Download or clone the repository to your local machine. </p>
<p>  Navigate to the root directory of the project in your terminal. </p>
<p>  Run the command <b>make build</b> to build the program </p>
<p>  Once the build is successful, you can run the program by using the command <b>./vma</b> </p>
<p>  To clean the generated files from the build process, use the command <b>make clean</b> </p>
<br>
<h2> 3) Usage </h2>

<p> <h3><b>ALLOC_ARENA {size}</b></h3> -> needs to be the first command written in the terminal; this command initialises the virtual arena and implies that all the memory blocks written afterwards need to be allocated inside it</p>

<p> <h3><b>DEALLOC_ARENA</h3></b> -> frees all the memory used by the arena and all of the blocks inside of it, including the data stored in them

<p> <h3><b>ALLOC_BLOCK {start_address} {size}</h3></b> -> allocates <u>size</u> bytes of memory in the arena, starting from the <u>start_address</u>. Each block has a miniblock list implemented in which the block is divided in more miniblocks that start at the <u>start_address</u>and all of their sizes added up are equal to the block's <u>size</u>.</p> 
<p><b>NOTE:</b> There are 3 cases of using this command that modifies the structure of the blocks: </p>
<p> -> block connects to the right side of another: miniblock is added to the end of left-side block's miniblock list and its size is now equal to the sum of both block's sizes; </p>
<p> -> block connects to the left side of another: miniblock is added to the start (head) of right-side block's miniblock list, its size is now equal to the sum of both block's sizes and the start address is now equal to the left-side block's start address; </p>
<p> -> block connects between 2 other blocks: miniblock is added in the middle of the left-side block's miniblock list, the miniblocks from the right side block's miniblock list get transferred at the end of left-side block's miniblock list and the right-side block is removed; at the end of this process there remains one single block with the size of all the previous blocks and 3 miniblocks in its list;

<p> <h3><b>FREE_BLOCK {start_address}</h3></b> -> frees the miniblock and all its data allocated at the <u>start_address</u>: </p>
<p> -> if the miniblock is the only one in the list, it removes its corresponding block with it; </p>
<p> -> if the miniblock is nor at the start, nor at the end of the list, the miniblock will be deleted and all the miniblocks after the deleted one will make up a new block with a new list; 

<p> <h3><b>WRITE {address} {size} {data}</h3></b> -> writes <u>size</u> bytes of <u>data</u> starting at the <u>address</u>; </p>
<p> -> the data will be automatically continued to another miniblock of the list if the miniblock's memory will be occupied; </p> 
<p><b>NOTE:</b> if the writing <u>size</u> is bigger than the remaining size of the block corresponding with the address, only the available writing size will be used and the remaining data will not be written; </p>

<p> <h3><b>READ {address} {size} {data}</h3></b> -> reads <u>size</u> bytes of <u>data</u> starting at the <u>address</u>; </p>
<p><b>NOTE:</b> if the reading <u>size</u> is bigger than the remaining size of the block corresponding with the address, only the available reading size will be read </p>

<p> <h3><b>PMAP</h3></b> -> shows all the useful information regarding the memory, displays all the blocks, their lists of miniblocks and the miniblocks' permissions (READ, WRITE, EXECUTE)</p>

<p> <h3><b>MPROTECT {change_permission}</h3></b> -> the default permissions for the miniblocks are <u>RW-</u> (READ and WRITE) </p>
The permissions can be changed by using these commands:
<p> -> <u>PROT_NONE</u>, <u>PROT_READ</u>, <u>PROT_WRITE</u>, <u>PROT_EXECUTE</u>
<p><b>NOTE:</b> The <u>READ</u> and <u>WRITE</u> permissions have direct connection with the <u>READ</u> and <u>WRITE</u> commands. Without these permissions the commands.</p>
<br>

<h2> 4) Examples </h2>

<p>ALLOC_ARENA 50</p>
<p>ALLOC_BLOCK 20 5</p>
<p>ALLOC_BLOCK 10 3</p>
<p>ALLOC_BLOCK 17 2</p>
<p>ALLOC_BLOCK 13 4</p>
<p>ALLOC_BLOCK 25 2</p>
<p>ALLOC_BLOCK 35 4</p>
<p>PMAP</p>
<br>
<div align = right>
<p><i>PMAP OUTPUT:</i> 
<p>Total memory: 0x32 bytes</p>
<p>Free memory: 0x1E bytes</p>
<p>Number of allocated blocks: 3</p>
<p>Number of allocated miniblocks: 6</p>
<br>
<p>Block 1 begin</p>
<p>Zone: 0xA - 0x13</p>
<p>Miniblock 1:            0xA             -               0xD             | RW-</p>
<p>Miniblock 2:            0xD             -               0x11            | RW-</p>
<p>Miniblock 3:            0x11            -               0x13            | RW-</p>
<p>Block 1 end</p>
<br>
<p>Block 2 begin</p>
<p>Zone: 0x14 - 0x1B</p>
<p>Miniblock 1:            0x14            -               0x19            | RW-</p>
<p>Miniblock 2:            0x19            -               0x1B            | RW-</p>
<p>Block 2 end</p>

<p>Block 3 begin</p>
<p>Zone: 0x23 - 0x27</p>
<p>Miniblock 1:            0x23            -               0x27            | RW-</p>
<p>Block 3 end</p>
</div>
<br>
<p>FREE_BLOCK 25</p>
<p>WRITE 13 10 helloworld</p>
<div align = right>
<p><i>WRITE OUTPUT:</i></p>
<p>Warning: size was bigger than the block size. Writing 6 characters.</p>
</div>
<br>
<p>READ 13 6</p>
<div align = right>
<p><i>READ OUTPUT:</i></p>
<p>hell</p>
</div>
<br>
<p>DEALLOC_ARENA</p>
<br>

<h2> 5) Troubleshooting </h2>

<p> -> <u>ALLOC_ARENA</u> needs to be the first used command </p>
<p> -> Can't free a block which wasn't allocated previously </p>
<p> -> Can't read a block which wasn't written in previously </p>
<p> -> Can't allocate memory in a zone already allocated </p>
<p> -> Can't allocate memory in a zone bigger than the arena </p>
<p> -> In case of <u>MPROTECT</u> applied, can't do an operation without the necessary permission (e.g. READ on a block with -WX permissions)</p> 
<br>

<h2> 6) Technical Details </h2>

<p>All the functions and methods used for making or using this program are detailed in the source code, the functions are explained in <u>vma.c</u> and the data structures used are explained in <u>vma.h</u>.</p>
