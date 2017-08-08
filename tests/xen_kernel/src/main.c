#include <stdio.h>
#include <string.h>
#include <x86/x86_64/hypercall-x86_64.h>


char hypercall_page[4096] __attribute__((aligned (4096))) __attribute__((section("text")));
static char string[] = "Hello";


static inline void wrmsrl(unsigned int msr, uint64_t val)
{
    uint32_t hi, lo;
    hi = (val >> 32);
    lo = (val & 0xffffffff);
    __asm__ __volatile__("wrmsr"
                         :
                         : "c" (msr), "d" (hi), "a" (lo)
                         );
}

static inline void hypercall(void)
{
    uint64_t op = 83;
    
    __asm__ __volatile__ ("vmcall"
                          :
                          : "D" ((long)string), "a" (op)
                          : "memory"
                          );
}

int main(int argc, const char **argv)
{
    (void) argc;
    (void) argv;

    printf("guest: %x\n", (long)hypercall_page);


    uint64_t cr3;
    __asm__ __volatile__ ("mov %%cr3, %0"
                          : "=a" (cr3)
                          : 
                          );

    printf("cr3: %d\n");

    hypercall();
    /*
    wrmsrl(0x40000000, (uint64_t)hypercall_page);
    if ((char *)hypercall_page == NULL) {
        printf("Hypercall_page is NULL, aborting...\n");
        goto abort;
    }
    */
    /*
    
    static char hello[] = "Hello!";
    HYPERVISOR_console_io(CONSOLEIO_write, strlen(hello), hello);
    */

 abort:
    return 0;
}
