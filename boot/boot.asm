ORG 0x7c00

BITS 16



CODE_SEG equ gdt_code - gdt_start

DATA_SEG equ gdt_data - gdt_start



_start:

    jmp short start

    nop



 times 33 db 0



start:

    jmp 0:step2



step2:

    cli ; Clear Interrupts

    mov ax, 0x00

    mov ds, ax

    mov es, ax

    mov ss, ax

    mov sp, 0x7c00

    sti ; Enables Interrupts



; ######### OS PRINT #######################

mov si, line1          ; load message into si for lodsb

call print

mov si, line2          ; load message into si for lodsb

call print

mov si, line3          ; load message into si for lodsb

call print

mov si, line4          ; load message into si for lodsb

call print

mov si, line5          ; load message into si for lodsb

call print

mov si, line6          ; load message into si for lodsb

call print



.load_protected:

    cli

    lgdt[gdt_descriptor]

    mov eax, cr0

    or eax, 0x1

    mov cr0, eax

    jmp CODE_SEG:load32



print:

	mov bx, 0

.loop:

	lodsb                    ; load byte in message and inc

	cmp al, 0

	je .done

	call print_char

	jmp .loop

.done:

	ret



print_char:

	mov ah, 0eh

	int 0x10

	ret





	; ######### GDT #############################

; GDT

gdt_start:

gdt_null:

    dd 0x0

    dd 0x0

	

gdt_code:     ; CS SHOULD POINT TO THIS

    dw 0xffff ; Segment limit first 0-15 bits

    dw 0      ; Base first 0-15 bits

    db 0      ; Base 16-23 bits

    db 0x9a   ; Access byte

    db 11001111b ; High 4 bit flags and the low 4 bit flags

    db 0        ; Base 24-31 bits



; offset 0x10

gdt_data:      ; DS, SS, ES, FS, GS

    dw 0xffff ; Segment limit first 0-15 bits

    dw 0      ; Base first 0-15 bits

    db 0      ; Base 16-23 bits

    db 0x92   ; Access byte

    db 11001111b ; High 4 bit flags and the low 4 bit flags

    db 0        ; Base 24-31 bits



gdt_end:

gdt_descriptor:

    dw gdt_end - gdt_start-1

    dd gdt_start

 

 [BITS 32]

 load32:

    mov eax, 1

    mov ecx, 100

    mov edi, 0x0100000

    call ata_lba_read

    jmp CODE_SEG:0x0100000



ata_lba_read:

    mov ebx, eax, ; Backup the LBA

    ; Send the highest 8 bits of the lba to hard disk controller

    shr eax, 24

    or eax, 0xE0 ; Select the  master drive

    mov dx, 0x1F6

    out dx, al

    ; Finished sending the highest 8 bits of the lba



    ; Send the total sectors to read

    mov eax, ecx

    mov dx, 0x1F2

    out dx, al

    ; Finished sending the total sectors to read



    ; Send more bits of the LBA

    mov eax, ebx ; Restore the backup LBA

    mov dx, 0x1F3

    out dx, al

    ; Finished sending more bits of the LBA



    ; Send more bits of the LBA

    mov dx, 0x1F4

    mov eax, ebx ; Restore the backup LBA

    shr eax, 8

    out dx, al

    ; Finished sending more bits of the LBA



    ; Send upper 16 bits of the LBA

    mov dx, 0x1F5

    mov eax, ebx ; Restore the backup LBA

    shr eax, 16

    out dx, al

    ; Finished sending upper 16 bits of the LBA



    mov dx, 0x1f7

    mov al, 0x20

    out dx, al



    ; Read all sectors into memory

.next_sector:

    push ecx



; Checking if we need to read

.try_again:

    mov dx, 0x1f7

    in al, dx

    test al, 8

    jz .try_again



; We need to read 256 words at a time

    mov ecx, 256

    mov dx, 0x1F0

    rep insw

    pop ecx

    loop .next_sector

    ; End of reading sectors into memory

    ret





;error_notice: db 'Failed to load sector!',10,13,0

line1: db '                   _ _____ _____ ',10,13,0

line2: db '                  (_)  _  /  ___|',10,13,0

line3: db ' _ __   __ ___   ___| | | \ `--. ',10,13,0

line4: db '| ',39,'_ \ / _` \ \ / / | | | |`--. \',10,13,0

line5: db '| | | | (_| |\ V /| \ \_/ /\__/ /',10,13,0

line6: db '|_| |_|\__,_| \_/ |_|\___/\____/',10,13,10,13,0



interrupt_notice: db 'Interrupted!',10,13,0

buffer:                      ; writing to here



times 510-($ - $$) db 0

dw 0xAA55  				     ; BIOS WILL NOT LOAD ANYTHING PAST THIS!

