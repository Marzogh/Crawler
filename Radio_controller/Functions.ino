void transmitCommand()
{
    radio.print(IO);
    radio.print(",");
    radio.print(mode);
    radio.print(",");
    radio.print(spd);
    radio.print(",");
    radio.print(brk);
    radio.print(",");
    radio.print(dir);
    radio.print(",");
    radio.println(res);
}
