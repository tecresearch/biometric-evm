document.addEventListener('DOMContentLoaded', function() {
    const logo = document.querySelector('.logo');
if (logo) {
    logo.style.cursor = 'pointer'; // Set cursor immediately when page loads
    logo.addEventListener('click', () => {
        window.location.href = 'http://localhost:5500/index.html';
    });
}
    
    // Tab switching functionality
    const passwordTab = document.getElementById('passwordTab');
    const fingerprintTab = document.getElementById('fingerprintTab');
    const passwordForm = document.getElementById('passwordForm');
    const fingerprintForm = document.getElementById('fingerprintForm');
    const switchToFingerprint = document.getElementById('switchToFingerprint');
    const switchToPassword = document.getElementById('switchToPassword');
    const fingerprintModal = document.getElementById('fingerprintModal');
    
    // Switch to fingerprint tab
    fingerprintTab.addEventListener('click', function() {
        switchTab('fingerprint');
    });
    
    switchToFingerprint.addEventListener('click', function() {
        switchTab('fingerprint');
    });
    
    // Switch to password tab
    passwordTab.addEventListener('click', function() {
        switchTab('password');
    });
    
    switchToPassword.addEventListener('click', function() {
        switchTab('password');
    });
    
    function switchTab(tab) {
        if (tab === 'fingerprint') {
            passwordTab.classList.remove('active');
            fingerprintTab.classList.add('active');
            passwordForm.classList.remove('active');
            fingerprintForm.classList.add('active');
            
            // Simulate fingerprint scanning after switching to fingerprint tab
            simulateFingerprintScan();
        } else {
            passwordTab.classList.add('active');
            fingerprintTab.classList.remove('active');
            passwordForm.classList.add('active');
            fingerprintForm.classList.remove('active');
        }
    }
    
    // Toggle password visibility
    const togglePassword = document.querySelector('.toggle-password');
    const passwordInput = document.getElementById('password');
    
    if (togglePassword && passwordInput) {
        togglePassword.addEventListener('click', function() {
            const type = passwordInput.getAttribute('type') === 'password' ? 'text' : 'password';
            passwordInput.setAttribute('type', type);
            togglePassword.innerHTML = type === 'password' ? '<i class="fas fa-eye"></i>' : '<i class="fas fa-eye-slash"></i>';
        });
    }
    
    // Form submission
    const forms = document.querySelectorAll('.login-form');
    forms.forEach(form => {
        form.addEventListener('submit', function(e) {
            e.preventDefault();
            
            if (form.id === 'passwordForm') {
                // Password form submission
                const username = document.getElementById('username').value;
                const password = document.getElementById('password').value;
                
                if (username && password) {
                    // Simulate authentication
                    simulatePasswordAuth();
                }
            } else if (form.id === 'fingerprintForm') {
                // Fingerprint form submission is handled by the scan simulation
            }
        });
    });
    
    // Simulate password authentication
    function simulatePasswordAuth() {
        const loginBtn = document.querySelector('#passwordForm .login-btn');
        const originalText = loginBtn.innerHTML;
        
        loginBtn.innerHTML = '<i class="fas fa-spinner fa-spin"></i> Authenticating...';
        loginBtn.disabled = true;
        
        // Simulate API call delay
        setTimeout(() => {
            // For demo purposes, always succeed
            window.location.href = '../dashboard/admin-panel.html';
        }, 1500);
    }
    
    // Simulate fingerprint scanning
    function simulateFingerprintScan() {
        const fingerprintStatus = document.querySelector('.fingerprint-status');
        const fingerprintAnimation = document.querySelector('.fingerprint-animation');
        
        fingerprintStatus.innerHTML = '<span class="status-pending">Scanning fingerprint...</span>';
        
        // Simulate scan delay
        setTimeout(() => {
            fingerprintStatus.innerHTML = '<span class="status-success">Fingerprint recognized!</span>';
            fingerprintAnimation.style.display = 'none';
            
            // Show success modal
            fingerprintModal.classList.add('active');
            
            // Redirect after delay
            setTimeout(() => {
                window.location.href = '../dashboard/admin-panel.html';
            }, 2000);
        }, 2500);
    }
    
    // Close modal if clicked outside
    fingerprintModal.addEventListener('click', function(e) {
        if (e.target === fingerprintModal) {
            fingerprintModal.classList.remove('active');
        }
    });
});