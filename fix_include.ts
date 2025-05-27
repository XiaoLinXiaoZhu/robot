// replaceIncludes.ts

//@ts-ignore
import { readdir, readFile, writeFile } from "fs/promises";
//@ts-ignore
import { join, extname } from "path";

// 设置项目根目录（请根据你的实际路径修改）
const projectDir = "E:/1统一下载位置/OTTOquad-master/firmwareVER_9"; // Windows 路径可用正斜杠

// 要查找和替换的内容
// const oldInclude = '#include <US.h>';
// const newInclude = '#include "Libraries/US/US.h"';
const replaceMap: Record<string, string> = {
    '#include "US.h"': '#include "Libraries/US/US.h"',
    '#include "US.cpp"': '#include "Libraries/US/US.cpp"',
    '#include "MaxMatrix.h"': '#include "Libraries/MaxMatrix/MaxMatrix.h"',
    '#include "MaxMatrix.cpp"': '#include "Libraries/MaxMatrix/MaxMatrix.cpp"',
    '#include <BatReader.h>': '#include "Libraries/BatReader/BatReader.h"',
    '#include "BatReader.h"': '#include "Libraries/BatReader/BatReader.h"',
    '#include <OttoSerialCommand.h>': '#include "Libraries/OttoSerialCommand/OttoSerialCommand.h"',
    '#include "OttoSerialCommand.h"': '#include "Libraries/OttoSerialCommand/OttoSerialCommand.h"',
};

// 要忽略的文件（避免修改 US 自身的源文件）
const ignoreFiles = ["US.h", "US.cpp", "MaxMatrix.h", "MaxMatrix.cpp", "BatReader.h", "BatReader.cpp", "OttoSerialCommand.h", "OttoSerialCommand.cpp"];

// 扩展名白名单
const allowedExtensions = [".h", ".cpp",".ino", ".c", ".cc", ".hpp"];

async function replaceInFile(filePath: string, fileName: string) {
  // 跳过黑名单中的文件
  if (ignoreFiles.includes(fileName)) {
    console.log(`🟡 跳过文件（在忽略列表中）: ${filePath}`);
    return;
  }

  // 检查扩展名是否符合要求
  const fileExt = extname(fileName);
  if (!allowedExtensions.includes(fileExt)) {
    console.log(`🟡 跳过文件（扩展名不匹配）: ${filePath}`);
    return;
  }

  try {
    let content = await readFile(filePath, "utf-8");

    let newContent = content;
    // 遍历替换映射
    for (const [oldInclude, newInclude] of Object.entries(replaceMap)) {
      newContent = newContent.replace(new RegExp(oldInclude, "g"), newInclude);
    }
    // 如果内容没有变化，直接返回
    if (newContent === content) {
      console.log(`🔵 无需修改（内容未变化）: ${filePath}`);
      return;
    }

    console.log(`🟢 正在修改文件: ${filePath}`)
    // 输出将要替换的内容（调试用）
    // for (const [oldInclude, newInclude] of Object.entries(replaceMap)) {
    // console.log(`   → 替换前: ${oldInclude}`);
    // console.log(`   → 替换后: ${newInclude}`);
    // }

    // console.log(`🔄 替换内容: ${content} -> ${newContent}`);

    // 写入修改后的内容
    console.log(`📝 写入修改后的内容到文件: ${filePath}`)

    // 写入文件
    await writeFile(filePath, newContent, "utf-8");

    console.log(`✅ 修改完成: ${filePath}`);
  } catch (err) {
    console.error(`❌ 读写文件出错: ${filePath}`, err);
  }
}

async function walkDir(dir: string) {
  console.log(`🔍 开始扫描目录: ${dir}`);
  const files = await readdir(dir);

  for (const file of files) {
    const fullPath = join(dir, file);

    // 只处理文件（非目录）
    try {
      // 简单判断是否为文件（Bun 方式）
      await Bun.file(fullPath).size;
      await replaceInFile(fullPath, file);
    } catch (e) {
      console.log(`📁 跳过目录（递归暂不支持）: ${fullPath}`);
    }
  }

  console.log("🏁 扫描并处理完成！");
}

// 启动脚本
await walkDir(projectDir);
console.log("🎉 替换任务已全部完成！");