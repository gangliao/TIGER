

<html>

<head>
<meta http-equiv="Content-Language" content="en-us">
<title>MIPS System Calls in SPIM</title>
</head>

<body>

<h3>MIPS system calls</h3>
(from <u>SPIM S20: A MIPS R2000 
Simulator</u>, James J. Larus, University of Wisconsin-Madison)
<br>
<br>

SPIM provides a small set of operating-system-like services
through the MIPS system call (syscall) instruction.
To request a service, a program loads the system call code
(see Table below) into register $v0 and the arguments into registers
$a0, ..., $a3 (or $f12 for floating point values).
System calls that return values put their result in register
$v0 (or $f0 for floating point results).

<table border="2" width="691">
  <tr>
    <td width="128"><b>Service</b></td>
    <td width="119"><b>System Call Code</b></td>
    <td width="267"><b>Arguments</b></td>
    <td width="147"><b>Result</b></td>
  </tr>
  <tr>
    <td width="128">print integer</td>
    <td align="middle" width="119">1</td>
    <td width="267">$a0 = value</td>
    <td width="147">(none)</td>
  </tr>
  <tr>
    <td width="128">print float</td>
    <td align="middle" width="119">2</td>
    <td width="267">$f12 = float value</td>
    <td width="147">(none)</td>
  </tr>
  <tr>
    <td width="128">print double</td>
    <td align="middle" width="119">3</td>
    <td width="267">$f12 = double value</td>
    <td width="147">(none)</td>
  </tr>
  <tr>
    <td width="128">print string</td>
    <td align="middle" width="119">4</td>
    <td width="267">$a0 = address of string</td>
    <td width="147">(none)</td>
  </tr>
  <tr>
    <td width="128">read integer</td>
    <td align="middle" width="119">5</td>
    <td width="267">(none)</td>
    <td width="147">$v0 = value read</td>
  </tr>
  <tr>
    <td width="128">read float</td>
    <td align="middle" width="119">6</td>
    <td width="267">(none)</td>
    <td width="147">$f0 = value read</td>
  </tr>
  <tr>
    <td width="128">read double</td>
    <td align="middle" width="119">7</td>
    <td width="267">(none)</td>
    <td width="147">$f0 = value read</td>
  </tr>
  <tr>
    <td width="128">read string</td>
    <td align="middle" width="119">8</td>
    <td width="267">$a0 = address where string to be stored<br>
    $a1 = number of characters to read + 1</td>
    <td width="147">(none)</td>
  </tr>
  <tr>
    <td width="128">memory allocation</td>
    <td align="middle" width="119">9</td>
    <td width="267">$a0 = number of bytes of storage desired</td>
    <td width="147">$v0 = address of block</td>
  </tr>
  <tr>
    <td width="128">exit (end of program)</td>
    <td align="middle" width="119">10</td>
    <td width="267">(none)</td>
    <td width="147">(none)</td>
  </tr>
  <tr>
    <td width="128">print character</td>
    <td align="middle" width="119">11</td>
    <td width="267">$a0 = integer</td>
    <td width="147">(none)</td>
  </tr>
  <tr>
    <td width="128">read character</td>
    <td align="middle" width="119">12</td>
    <td width="267">(none)</td>
    <td width="147">char in $v0</td>
  </tr>
</table>

For example, to print "<tt>the answer = 5</tt>", use the commands:

<blockquote>
<pre>
.data
str: .asciiz "the answer = "
.text
    li $v0, 4       # $system call code for print_str
    la $a0, str     # $address of string to print
    syscall         # print the string

    li $v0, 1       # $system call code for print_int
    li $a0, 5       # $integer to print
    syscall         # print it
</pre>
</blockquote>

<ul>
<li><b>print int</b> passes an integer and prints it on the console.</li>
<li><b>print float</b> prints a single floating point number.</li>
<li><b>print double</b> prints a double precision number.</li>
<li><b>print string</b> passes a pointer to a null-terminated string</li>
<li><b>read int</b>, <b>read float</b>, and <b>read double</b> read
  an entire line of input up to and including a newline.
<li><b>read string</b> has the same semantics as the Unix
  library routine fgets.
  It reads up to n - 1 characters into a buffer
  and terminates the string with a null byte.
  If there are fewer characters on the current line,
  it reads through the newline and again null-terminates the string.</li>
<li><b>sbrk</b> returns a pointer to a block of memory
containing <i>n</i> additional bytes.</li>
<li><b>exit</b> stops a program from running.</li>
</ul>

</body>

</html>
