Sample Virus Scanner

This virus scanner is written on a Linux machine as a C code.
To start scanning for malware first we need to build the signature binary file from the list of signature.
To achieve that we use the binary compile from 'createsigs.c' as below:

	./createsigs signatures.txt

The above command will take the signatures.txt file and write a signatures.bin file which we will use as input for the virus scanner.
Next, we need to create the virus binary. To do that we compile the malicious code of 'createsigs.c'.
If we run the binary compiled from this code it will start a netcat server for everyone to listen to which we are using as a signature.
So the pattern we are looking for for this virus is 'netcat'. We compile this code to generate the virus binary as below:

	gcc createvirus.c -o thisisvirus.bin

Once we have the signature and virus binaries we can start scanning with the scanner tool 'virusscanner' compiled from virusscanner.c.
This scanner takes two inputs, the directory to search and the signature file and writes the scan results in 'results.txt'.

	./virusscanner /root/scanthis/ signatures.bin

The scanner works by calling the function 'FindFilesAndScan'.
This function uses 'opendir' and 'readdir' to iterate through all the subdirectories and files.
To do that first we read the input directory and open the directory stream using opendir.
We use this stream in readdir to read the contents of the directory one by one.
We check if the chosen path is a directory, if yes the 'FindFilesAndScan' function is called again recursively to get the contents of this subdirectory.
If the path from readdir is a file then we call the virus scanner function 'scan' on that file.
The scan function first opens both the file to scan and the signature file and read from them byte by byte using fread in loop.
Within the loop we keep track of the number of bytes traversed for the scanning file and we move on to next signature, identified by the preceding newline character, using fseek if the current signature didn't match for the file.

When we move to next signature we update a counter with the offset of the signature in the signature file stream so that we can access the signature again to print in results if a match is found.
We perform the matching of signatures with the file byte by byte and increment a counter for each matched byte.
If the match fails we reset the file pointer of the signature file stream to the start of the current signature and start matching the file from next byte onwards in the file stream.
When the scan completes the scanner will write the list of infected files with the signature it matched to into the 'results.txt' file.
