# Ham dung chung cho cac script dong bo du lieu.
# Dung System.Data.SqlClient co san trong Windows PowerShell, khong can cai sqlcmd hay SSMS.

function Mo-KetNoi([string]$Server, [string]$Db = 'master') {
    $chuoi = "Server=$Server;Database=$Db;Integrated Security=True;TrustServerCertificate=True;Connect Timeout=15"
    $cn = New-Object System.Data.SqlClient.SqlConnection $chuoi
    $cn.Open()
    return $cn
}

function Chay-Sql($cn, [string]$sql) {
    $lenh = $cn.CreateCommand()
    $lenh.CommandText = $sql
    $lenh.CommandTimeout = 600
    [void]$lenh.ExecuteNonQuery()
}

function Lay-GiaTri($cn, [string]$sql) {
    $lenh = $cn.CreateCommand()
    $lenh.CommandText = $sql
    return $lenh.ExecuteScalar()
}

function Lay-Bang($cn, [string]$sql) {
    $lenh = $cn.CreateCommand()
    $lenh.CommandText = $sql
    $bang = New-Object System.Data.DataTable
    [void](New-Object System.Data.SqlClient.SqlDataAdapter $lenh).Fill($bang)
    return , $bang
}

# Chia script SQL theo dong "GO" (giong SSMS/sqlcmd).
function Tach-Lo([string]$noiDung) {
    return [regex]::Split($noiDung, '(?im)^[ \t]*GO[ \t]*\r?$') | Where-Object { $_.Trim() -ne '' }
}
