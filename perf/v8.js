function heap_insert(heap, item, cmp) {
    var child_index = heap.length;
    while (child_index > 0) {
        var parent_index = Math.floor((child_index - 1) / 2);
        var parent_item = heap[parent_index];
        if (cmp(item, parent_item)) {
            heap[child_index] = parent_item;
        } else {
            break;
        }
        child_index = parent_index;
    }
    heap[child_index] = item;
}

function heap_pop(heap, cmp) {
    var res = heap[0];

    var last = heap.pop();

    var len = heap.length;
    if (!len) {
        return last;
    }

    var parent_index = 0;
    while(parent_index * 2 + 1 < len) {
        var child_index = parent_index * 2 + 1;
        if (child_index + 1 < len && cmp(heap[child_index+1], heap[child_index])) {
            ++child_index;
        }
        var child_rec = heap[child_index];
        if (cmp(last, child_rec)) {
            break;
        } else {
            heap[parent_index] = child_rec;
            parent_index = child_index;
        }
    }
    heap[parent_index] = last;

    return res;
}

function cmp(a, b) {
    return a[0] < b[0];
}

function getTop(n) {
    var size = data_size();
    var counts = {};

    for (var i=0; i < size; ++i) {
        var s = data_elem(i);
        counts[s] = (counts[s] || 0) + 1
    }

    var tops = [];

    for (var s in counts) {
        var c = counts[s];
        if (tops.length < n) {
            tops.push([c, s]);
        } else if (tops[0][0] < c) {
            heap_pop(tops, cmp);
            heap_insert(tops, [c, s], cmp);
        }
    }

    var ret = [];
    while (tops.length) {
        var e = heap_pop(tops, cmp);
        ret.push(e[0] + ': ' + e[1]);
    }

    return ret.join('\n') + '\n';
}

getTop(100);
