#!/bin/sh
# 拉取第三方阅读材料（论文/手册，版权属原作者，不随仓库分发）
# 来源: https://pdos.csail.mit.edu/6.1810/2024/readings/
set -e
BASE="https://pdos.csail.mit.edu/6.1810/2024/readings"
DEST="${1:-readings}"
FILES="8254x_GBe_SDM.pdf
FU540-C000-v1.0.pdf
appel-li.pdf
belay-dune.pdf
journal-design.pdf
journal-ext2fs.html
meltdown.pdf
microkernel.pdf
mogul96usenix.pdf
rcu-decade-later.pdf
redleaf.pdf
riscv-calling.pdf
ritchie78unix.pdf
virtio-v1.1-csprd01.pdf"

mkdir -p "$DEST"
for f in $FILES; do
    if [ -f "$DEST/$f" ]; then
        echo "skip $f"
        continue
    fi
    echo "fetch $f"
    curl -fL --retry 3 -o "$DEST/$f" "$BASE/$f"
done
echo "done -> $DEST/"
