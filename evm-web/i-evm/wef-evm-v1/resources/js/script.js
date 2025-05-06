document.addEventListener('DOMContentLoaded', function() {
    // Initialize Charts
    initCharts();
    
    // Modal functionality : Adding Candidate
    const addCandidateBtn = document.querySelector('.btn-primary');
    const addCandidateModal = document.getElementById('addCandidateModal');
    const closeModalBtns = document.querySelectorAll('.close-modal');
    
    if (addCandidateBtn) {
        addCandidateBtn.addEventListener('click', function() {
            addCandidateModal.classList.add('active');
        });
    }
    
    closeModalBtns.forEach(btn => {
        btn.addEventListener('click', function() {
            addCandidateModal.classList.remove('active');
        });
    });
    
    // Close modal when clicking outside
    addCandidateModal.addEventListener('click', function(e) {
        if (e.target === addCandidateModal) {
            addCandidateModal.classList.remove('active');
        }
    });
    
    // Form submission
    const addCandidateForm = document.getElementById('addCandidateForm');
    if (addCandidateForm) {
        addCandidateForm.addEventListener('submit', function(e) {
            e.preventDefault();
            // Simulate form submission
            const fingerprintStatus = document.querySelector('.fingerprint-status');
            fingerprintStatus.innerHTML = '<span class="status-success">Fingerprint registered successfully!</span>';
            
            setTimeout(() => {
                addCandidateModal.classList.remove('active');
                alert('Candidate registered successfully!');
                addCandidateForm.reset();
                fingerprintStatus.innerHTML = '<span class="status-pending">Waiting for fingerprint...</span>';
            }, 1500);
        });
    }
    
    // Simulate fingerprint registration
    const fingerprintRegister = document.querySelector('.fingerprint-register');
    if (fingerprintRegister) {
        fingerprintRegister.addEventListener('click', function() {
            const fingerprintStatus = document.querySelector('.fingerprint-status');
            fingerprintStatus.innerHTML = '<span class="status-success">Fingerprint registered successfully!</span>';
        });
    }

    // Modal functionality : Adding Voter

    const addVoterBtn=document.querySelector('#voter-btn');
    const addVoterModal=document.querySelector('#addVoterModal');
    if (addVoterBtn) {
        addVoterBtn.addEventListener('click', function() {
            addVoterModal.classList.add('active');
        });
    }

    closeModalBtns.forEach(btn => {
        btn.addEventListener('click', function() {
            addVoterModal.classList.remove('active');
        });
    });
    
     // Close modal when clicking outside
     addVoterModal.addEventListener('click', function(e) {
        if (e.target === addVoterModal) {
            addVoterModal.classList.remove('active');
        }
    });



         // Simulate fingerprint registration for voter
    const fingerprintRegisterVoter = document.querySelector('.fingerprint-register-voter');
    if (fingerprintRegisterVoter) {
        fingerprintRegisterVoter.addEventListener('click', function() {
            const fingerprintStatusVoter = document.querySelector('.fingerprint-status-voter');
            fingerprintStatusVoter.innerHTML = '<span class="status-success">Fingerprint registered successfully!</span>';
        });
    }


      // Form submission
      const addVoterForm = document.getElementById('addVoterForm');
      if (addVoterForm) {
       addVoterForm.addEventListener('submit', function(e) {
              e.preventDefault();
              // Simulate form submission
              const fingerprintStatus = document.querySelector('.fingerprint-status');
              fingerprintStatus.innerHTML = '<span class="status-success">Fingerprint registered successfully!</span>';
              
              setTimeout(() => {
               addVoterModal.classList.remove('active');
                  alert('Voter registered successfully!');
                  addVoterForm.reset();
                  fingerprintStatus.innerHTML = '<span class="status-pending">Waiting for fingerprint...</span>';
              }, 1500);
          });
      }

    // Logout button
    const logoutBtn = document.querySelector('.logout-btn');
    if (logoutBtn) {
        logoutBtn.addEventListener('click', function() {
            if (confirm('Are you sure you want to logout?')) {
                // Redirect to login page
                window.location.href = '../pages/login.html';
            }
        });
    }
});

function initCharts() {
    // Pie Chart - Vote Distribution
    const pieCtx = document.getElementById('pieChart').getContext('2d');
    const pieChart = new Chart(pieCtx, {
        type: 'pie',
        data: {
            labels: ['Brijesh Nishad', 'Priya Sharma', 'Rahul Verma', 'Anjali Patel'],
            datasets: [{
                data: [230, 158, 132, 98],
                backgroundColor: [
                    '#138808',
                    '#FF9933',
                    '#000080',
                    '#9400D3'
                ],
                borderWidth: 1
            }]
        },
        options: {
            responsive: true,
            maintainAspectRatio: false,
            plugins: {
                legend: {
                    position: 'right',
                },
                tooltip: {
                    callbacks: {
                        label: function(context) {
                            const label = context.label || '';
                            const value = context.raw || 0;
                            const total = context.dataset.data.reduce((a, b) => a + b, 0);
                            const percentage = Math.round((value / total) * 100);
                            return `${label}: ${value} votes (${percentage}%)`;
                        }
                    }
                }
            }
        }
    });
    
    // Line Chart - Voting Trend
    const lineCtx = document.getElementById('lineChart').getContext('2d');
    const lineChart = new Chart(lineCtx, {
        type: 'line',
        data: {
            labels: ['9:00', '10:00', '11:00', '12:00', '13:00', '14:00', '15:00'],
            datasets: [
                {
                    label: 'Brijesh Nishad',
                    data: [30, 45, 60, 90, 120, 180, 230],
                    borderColor: '#138808',
                    backgroundColor: 'rgba(19, 136, 8, 0.1)',
                    tension: 0.3,
                    fill: true
                },
                {
                    label: 'Priya Sharma',
                    data: [20, 35, 50, 70, 100, 130, 158],
                    borderColor: '#FF9933',
                    backgroundColor: 'rgba(255, 153, 51, 0.1)',
                    tension: 0.3,
                    fill: true
                },
                {
                    label: 'Rahul Verma',
                    data: [15, 30, 45, 65, 90, 115, 132],
                    borderColor: '#000080',
                    backgroundColor: 'rgba(0, 0, 128, 0.1)',
                    tension: 0.3,
                    fill: true
                },
                {
                    label: 'Anjali Patel',
                    data: [10, 20, 35, 50, 70, 85, 98],
                    borderColor: '#9400D3',
                    backgroundColor: 'rgba(148, 0, 211, 0.1)',
                    tension: 0.3,
                    fill: true
                }
            ]
        },
        options: {
            responsive: true,
            maintainAspectRatio: false,
            plugins: {
                legend: {
                    position: 'top',
                },
                tooltip: {
                    mode: 'index',
                    intersect: false,
                }
            },
            scales: {
                y: {
                    beginAtZero: true,
                    title: {
                        display: true,
                        text: 'Number of Votes'
                    }
                },
                x: {
                    title: {
                        display: true,
                        text: 'Time'
                    }
                }
            }
        }
    });
}