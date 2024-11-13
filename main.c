#include <stdio.h>
#include <stdlib.h>

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main(int argc, char *argv[]) {
	printf("%d",sizeof(int));
	return 0;
}
//converts bit-stream into 32-bit integer chunk of data
//This function save previous results in an static array
unsigned int streamIIRFilter(unsigned char* bitStream,  unsigned int * intData){
	static unsigned int filterWindow 
}

const fs = require('fs');
//blackman 4 dig prec
let coefs= [  0.0,1.159e-07,7.864e-07,2.523e-06,5.88e-06,1.146e-05,1.993e-05,3.202e-05,4.852e-05,7.03e-05,
        9.831e-05,1.336e-04,1.773e-04,2.305e-04,2.946e-04,3.709e-04,4.609e-04,5.659e-04,6.874e-04,8.271e-04,
        9.865e-04,1.167e-03,1.37e-03,1.598e-03,1.85e-03,2.13e-03,2.438e-03,2.774e-03,3.14e-03,3.536e-03,
        3.963e-03,4.421e-03,4.909e-03,5.427e-03,5.975e-03,6.551e-03,7.154e-03,7.783e-03,8.434e-03,9.107e-03,
        9.797e-03,1.05e-02,1.122e-02,1.195e-02,1.268e-02,1.341e-02,1.414e-02,1.486e-02,1.556e-02,1.625e-02,
        1.692e-02,1.757e-02,1.818e-02,1.876e-02,1.93e-02,1.98e-02,2.026e-02,2.066e-02,2.102e-02,2.132e-02,
        2.156e-02,2.174e-02,2.186e-02,2.192e-02,2.192e-02,2.186e-02,2.174e-02,2.156e-02,2.132e-02,2.102e-02,
        2.066e-02,2.026e-02,1.98e-02,1.93e-02,1.876e-02,1.818e-02,1.757e-02,1.692e-02,1.625e-02,1.556e-02,
        1.486e-02,1.414e-02,1.341e-02,1.268e-02,1.195e-02,1.122e-02,1.05e-02,9.797e-03,9.107e-03,8.434e-03,
        7.783e-03,7.154e-03,6.551e-03,5.975e-03,5.427e-03,4.909e-03,4.421e-03,3.963e-03,3.536e-03,3.14e-03,
        2.774e-03,2.438e-03,2.13e-03,1.85e-03,1.598e-03,1.37e-03,1.167e-03,9.865e-04,8.271e-04,6.874e-04,
        5.659e-04,4.609e-04,3.709e-04,2.946e-04,2.305e-04,1.773e-04,1.336e-04,9.831e-05,7.03e-05,4.852e-05,
        3.202e-05,1.993e-05,1.146e-05,5.88e-06,2.523e-06,7.864e-07,1.159e-07,0.0];
      coefs =   coefs.map((val,idx)=>Math.round(val*65535)); // 524280 >> 3 = 65535
let another = Array.from(coefs)

for (let q=coefs.length-1; q>=0; q--){
    another.push(coefs[q]);
}
let content='['
let sum=0;
another.forEach((v,idx)=>{
    content +=`${v},`
     sum += v
    })
    console.log(sum)
content += '], shitf right on 3 bits to have 65535 in sum';
fs.writeFileSync('./Koefs.txt',content);