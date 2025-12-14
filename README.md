# Compress
基于 Huffman 编码的二进制压缩/解压工具<br>
支持对输入文件进行 Huffman 压缩，生成自定义扩展名文件<br>

# 压缩<br>
compress.exe -c input.txt output.*<br>

# 解压<br>
compress.exe -d output.*  restored.txt<br>

#文件头<br>
    head：
        -magic：标识文件
        -total：原始数据总字节数
        -sz：频率表条目数
        -freq table：频率表
        -data bits：Huffman 编码后的比特流
        -valid：最后 1 字节记录有效位