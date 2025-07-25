#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <iomanip>

std::size_t compute_AC_index_size(const std::string& filename,
                                  std::size_t int_bytes  = 4,
                                  std::size_t null_bytes = 8)
{
    std::ifstream fin(filename, std::ios::binary);
    if (!fin.is_open()) {
        std::cerr << "Error: cannot open " << filename << '\n';
        return 0;
    }

    std::size_t int_cnt  = 0;
    std::size_t null_cnt = 0;
    std::string line;

    const std::regex num_re(R"(\d+)");
    while (std::getline(fin, line)) {
        // 缁熻鏁板瓧
        for (auto it = std::sregex_iterator(line.begin(), line.end(), num_re);
             it != std::sregex_iterator(); ++it)
            ++int_cnt;

        // 缁熻 null
        std::size_t pos = 0;
        while ((pos = line.find("null", pos)) != std::string::npos) {
            ++null_cnt;
            pos += 4;
        }
    }

    std::size_t bytes = int_cnt * int_bytes ;

    std::cout << "ints  : " << int_cnt
              << " 脳 " << int_bytes  << "B = " << int_cnt  * int_bytes  << " B\n"
              << "nulls : " << null_cnt
              << " 脳 " << null_bytes << "B = " << null_cnt * null_bytes << " B\n"
              << "---------------------------------------------\n"
              << "Total : " << bytes << " B  ("
              << std::fixed << std::setprecision(2)
              << bytes / 1024.0        << " KB, "
              << bytes / 1024.0 / 1024 << " MB)\n";

    return bytes;
}
std::size_t compute_VC_index_size(const std::string& filename,
                                  std::size_t int_bytes  = 4,
                                  std::size_t null_bytes = 8)
{
    std::ifstream fin(filename, std::ios::binary);
    if (!fin.is_open()) {
        std::cerr << "Error: cannot open " << filename << '\n';
        return 0;
    }

    std::size_t int_cnt  = 0;
    std::size_t null_cnt = 0;
    std::string line;

    // 姝ｅ垯鐢ㄤ簬鎵炬暟瀛?
    const std::regex num_re(R"(\d+)");
    while (std::getline(fin, line)) {
        /* 1. 缁熻鏁板瓧 */
        for (auto it = std::sregex_iterator(line.begin(), line.end(), num_re);
             it != std::sregex_iterator(); ++it)
            ++int_cnt;

        /* 2. 缁熻 "null" 鈥斺€?绠€鍗曞瓙涓叉悳绱㈠嵆鍙?*/
        std::size_t pos = 0;
        while ((pos = line.find("null", pos)) != std::string::npos) {
            ++null_cnt;
            pos += 4;                // 璺宠繃 "null"
        }
    }

    std::size_t bytes = int_cnt * int_bytes;

    std::cout << "ints  : " << int_cnt
              << " 脳 " << int_bytes  << "B = " << int_cnt  * int_bytes  << " B\n"
              << "nulls : " << null_cnt
              << " 脳 " << null_bytes << "B = " << null_cnt * null_bytes << " B\n"
              << "---------------------------------------------\n"
              << "Total : " << bytes << " B  ("
              << std::fixed << std::setprecision(2)
              << bytes / 1024.0        << " KB, "
              << bytes / 1024.0 / 1024 << " MB)\n";

    return bytes;
}
int estimateLineSize(int i1, int i2, const string& s1, const string& s2) {
    int intSize = 4;
    return intSize * 4;  // 假设字符串为英文字符，每字符1字节
}

size_t calculateFileSize(const string& filename) {
    ifstream infile(filename);
    if (!infile.is_open()) {
        cerr << "无法打开文件！" << endl;
        return 0;
    }

    string line;
    size_t totalSize = 0;

    while (getline(infile, line)) {
        istringstream iss(line);
        int a, b;
        string s1, s2;
        if (iss >> a >> b >> s1 >> s2) {
            totalSize += estimateLineSize(a, b, s1, s2);
        }
    }

    infile.close();
    cout<<"文件大小为："<<totalSize/1024.0<<"KB"<<endl;
    return totalSize;
}

// int main()
// {
//     std::string idx_num = "1228";  // 这里改成你要的索引编号
//     std::vector<std::string> ratios = {"20", "40", "60", "80"};

// for (const auto& ratio : ratios) {
    
//        std::vector<std::string> AC_files = {
//             "AC_u_uindex_" + idx_num + "_" + ratio + ".txt",
//             "AC_u_vindex_" + idx_num + "_" + ratio + ".txt",
//             "AC_v_uindex_" + idx_num + "_" + ratio + ".txt",
//             "AC_v_vindex_" + idx_num + "_" + ratio + ".txt"
//         };

//         // 构造 MAC 文件名
//         std::vector<std::string> MAC_files = {
//             "MAC_u_uindex_" + idx_num + "_" + ratio + ".txt",
//             "MAC_u_vindex_" + idx_num + "_" + ratio + ".txt",
//             "MAC_v_uindex_" + idx_num + "_" + ratio + ".txt",
//             "MAC_v_vindex_" + idx_num + "_" + ratio + ".txt"
//         };

//     std::vector<std::string> VC_files = {
//         "VC_u_"+idx_num+".txt",
//         "VC_v_"+idx_num+".txt"
//     };
 
//     std::size_t total_AC = 0;
//     std::cout << "Processing AC files:\n";
//     for (const auto& file : AC_files) {
//         std::cout << "File: " << file << std::endl;
//         total_AC += compute_AC_index_size(file);
//     }
//     std::cout << "---------------------------------------------\n";
//     std::cout << "Total size of AC files: "
//               << total_AC << " B ("
//               << std::fixed << std::setprecision(2)
//               << total_AC / 1024.0 << " KB, "
//               << total_AC / 1024.0 / 1024 << " MB)\n\n";

//     std::size_t total_MAC = 0;
//     std::cout << "Processing MAC files:\n";
//     for (const auto& file : MAC_files) {
//         std::cout << "File: " << file << std::endl;
//         total_MAC += compute_AC_index_size(file);  // 如果你有专门 MAC 函数可以换
//     }
//     std::cout << "---------------------------------------------\n";
//     std::cout << "Total size of MAC files: "
//               << total_MAC << " B ("
//               << std::fixed << std::setprecision(2)
//               << total_MAC / 1024.0 << " KB, "
//               << total_MAC / 1024.0 / 1024 << " MB)\n\n";

//     std::cout << "=============================================\n";
//     std::cout << "Combined total size: "
//               << total_AC + total_MAC << " B ("
//               << std::fixed << std::setprecision(2)
//               << (total_AC + total_MAC) / 1024.0 << " KB, "
//               << (total_AC + total_MAC) / 1024.0 / 1024 << " MB)\n";
             

     
//         // 统计VC文件大小
//     std::size_t total_VC = 0;
//     std::cout << "Processing VC files:\n";
//     for (const auto& file : VC_files) {
//         std::cout << "File: " << file << std::endl;
//         total_VC += compute_VC_index_size(file);  // VC用这个函数
//     }
//     std::cout << "---------------------------------------------\n";
//     std::cout << "Total size of VC files: "
//               << total_VC << " B ("
//               << std::fixed << std::setprecision(2)
//               << total_VC / 1024.0 << " KB, "
//               << total_VC / 1024.0 / 1024 << " MB)\n\n";
    
//    std::cout << "[" << total_AC/ 1024.0 << ", " << total_VC/ 1024.0 << ", " << total_MAC/ 1024.0 << "] // " << "%\n";
    
// }
//     return 0;
// }

