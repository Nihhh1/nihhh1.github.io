class Library {
    constructor() {
        this.files = [];
        this.currentCategory = '全部';
        this.searchTerm = '';

        this.init();
    }

    init() {
        // 初始化搜索功能
        const searchInput = document.getElementById('library-search-input');
        if (searchInput) {
            searchInput.addEventListener('input', (e) => this.handleSearch(e.target.value));
        }

        // 初始化分类点击事件
        const categories = document.querySelectorAll('.library-category');
        categories.forEach(category => {
            category.addEventListener('click', () => this.handleCategoryClick(category));
        });

        // 加载初始文件列表
        this.loadFiles();
    }

    async loadFiles() {
        try {
            const response = await fetch('/api/files.json');
            if (!response.ok) throw new Error('Failed to load files');

            this.files = await response.json();
            this.renderFiles();
        } catch (error) {
            console.error('Error loading files:', error);
            this.showError('加载文件列表失败');
        }
    }

    handleSearch(term) {
        this.searchTerm = term.toLowerCase();
        this.renderFiles();
    }

    handleCategoryClick(categoryElement) {
        // 更新分类样式
        document.querySelectorAll('.library-category').forEach(c => {
            c.classList.remove('active');
        });
        categoryElement.classList.add('active');

        // 更新当前分类并重新渲染
        this.currentCategory = categoryElement.textContent;
        this.renderFiles();
    }

    filterFiles() {
        return this.files.filter(file => {
            const matchesSearch = this.searchTerm === '' ||
                file.name.toLowerCase().includes(this.searchTerm);
            const matchesCategory = this.currentCategory === '全部' ||
                file.category === this.currentCategory;

            return matchesSearch && matchesCategory;
        });
    }

    renderFiles() {
        const libraryList = document.querySelector('.library-list');
        if (!libraryList) return;

        const filteredFiles = this.filterFiles();

        if (filteredFiles.length === 0) {
            libraryList.innerHTML = `
                <div class="library-empty">
                    <i class="fa fa-folder-open-o"></i>
                    <p>没有找到匹配的文件</p>
                </div>
            `;
            return;
        }

        libraryList.innerHTML = filteredFiles.map(file => this.createFileItemHTML(file)).join('');

        // 绑定文件操作事件
        this.bindFileActions();
    }

    createFileItemHTML(file) {
        return `
            <div class="library-file-item" data-file-id="${file.id}">
                <div class="library-file-icon">
                    <i class="fa ${this.getFileIcon(file.type)}"></i>
                </div>
                <div class="library-file-info">
                    <div class="library-file-name">${file.name}</div>
                    <div class="library-file-meta">
                        <span>${file.size}</span> | 
                        <span>上传于 ${file.uploadDate}</span>
                    </div>
                </div>
                <div class="library-file-actions">
                    <button class="library-file-action library-file-preview" onclick="library.previewFile('${file.id}')">
                        <i class="fa fa-eye"></i> 预览
                    </button>
                    <button class="library-file-action library-file-download" onclick="library.downloadFile('${file.id}')">
                        <i class="fa fa-download"></i> 下载
                    </button>
                </div>
            </div>
        `;
    }

    getFileIcon(fileType) {
        const iconMap = {
            'pdf': 'fa-file-pdf-o',
            'doc': 'fa-file-word-o',
            'docx': 'fa-file-word-o',
            'xls': 'fa-file-excel-o',
            'xlsx': 'fa-file-excel-o',
            'ppt': 'fa-file-powerpoint-o',
            'pptx': 'fa-file-powerpoint-o',
            'jpg': 'fa-file-image-o',
            'jpeg': 'fa-file-image-o',
            'png': 'fa-file-image-o',
            'gif': 'fa-file-image-o',
            'zip': 'fa-file-archive-o',
            'rar': 'fa-file-archive-o',
            'txt': 'fa-file-text-o'
        };

        return iconMap[fileType.toLowerCase()] || 'fa-file-o';
    }

    bindFileActions() {
        // 预览和下载按钮的事件已通过onclick属性绑定
    }

    async previewFile(fileId) {
        try {
            const file = this.files.find(f => f.id === fileId);
            if (!file) throw new Error('File not found');

            // 根据文件类型实现不同的预览方式
            if (file.type === 'pdf') {
                window.open(`/preview/${fileId}`, '_blank');
            } else if (['jpg', 'jpeg', 'png', 'gif'].includes(file.type.toLowerCase())) {
                // 显示图片预览模态框
                this.showImagePreview(file);
            } else {
                alert('该文件类型暂不支持预览');
            }
        } catch (error) {
            console.error('Error previewing file:', error);
            this.showError('预览文件失败');
        }
    }

    async downloadFile(fileId) {
        try {
            const response = await fetch(`/api/download/${fileId}`);
            if (!response.ok) throw new Error('Download failed');

            const blob = await response.blob();
            const url = window.URL.createObjectURL(blob);
            const a = document.createElement('a');
            a.href = url;
            a.download = this.files.find(f => f.id === fileId).name;
            document.body.appendChild(a);
            a.click();
            window.URL.revokeObjectURL(url);
            document.body.removeChild(a);
        } catch (error) {
            console.error('Error downloading file:', error);
            this.showError('下载文件失败');
        }
    }

    showError(message) {
        // 实现错误提示UI
        alert(message);
    }

    showImagePreview(file) {
        // 实现图片预览模态框
        const modal = document.createElement('div');
        modal.className = 'library-modal';
        modal.innerHTML = `
            <div class="library-modal-content">
                <span class="library-modal-close">&times;</span>
                <img src="/files/${file.id}" alt="${file.name}">
            </div>
        `;

        modal.querySelector('.library-modal-close').onclick = () => {
            document.body.removeChild(modal);
        };

        document.body.appendChild(modal);
    }
}

// 初始化文件库
document.addEventListener('DOMContentLoaded', () => {
    window.library = new Library();
}); 