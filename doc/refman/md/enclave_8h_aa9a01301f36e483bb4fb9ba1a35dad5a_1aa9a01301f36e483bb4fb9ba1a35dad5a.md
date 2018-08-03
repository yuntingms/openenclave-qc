[Index](index.md)

---
# oe_call_host()

Perform a high-level enclave function call (OCALL).

## Syntax

    oe_result_t oe_call_host(const char *func, void *args)
## Description 

Call the host function whose name is given by the **func** parameter. The host must define a corresponding function with the following prototype.

```
OE_OCALL void (*)(void* args);
```



The meaning of the **args** parameter is defined by the implementer of the function and may be null.

This function is implemented using the low-level oe_ecall() interface where the function number is given by the **OE_OCALL_CALL_HOST** constant.

Note that the return value of this function only indicates the success of the call and not of the underlying function. The OCALL implementation must define its own error reporting scheme based on **args**.



## Parameters

#### func

The name of the enclave function that will be called.

#### args

The arguments to be passed to the enclave function.

## Returns

This function return **OE_OK** on success.

---
[Index](index.md)
