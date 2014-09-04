$addr = "http://the.earth.li/~sgtatham/putty/latest/x86/"
$execs = @("pscp","plink","putty","puttygen","pageant")
$path = (Get-Item -Path ".\" -Verbose).FullName
$client = new-object System.Net.WebClient

foreach ($exec in $execs) {
    echo "Getting $($addr)$($exec).exe"
    $client.DownloadFile("$addr$exec.exe","$path\$exec.exe")
}
