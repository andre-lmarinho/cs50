// Function to load external HTML into a container
function includeHTML(id, url) {
  fetch(url)
    .then(response => {
      if (!response.ok) {
        throw new Error(`Failed to load ${url}: ${response.status}`);
      }
      return response.text();
    })
    .then(html => {
      document.getElementById(id).innerHTML = html;
    })
    .catch(err => console.error(err));
}

// Smooth scroll for arrow-down button
function initSmoothScroll() {
  const arrow = document.querySelector('.arrow-down');
  if (!arrow) return;

  arrow.addEventListener('click', (event) => {
    event.preventDefault();
    const target = document.getElementById('about');
    if (target) {
      target.scrollIntoView({ behavior: 'smooth', block: 'start' });
    }
  });
}

// Initialize Bootstrap tooltips
function initTooltips() {
  const tooltipTriggerList = [].slice.call(document.querySelectorAll('[data-toggle="tooltip"]'));
  tooltipTriggerList.forEach(tooltipTriggerEl => {
    new bootstrap.Tooltip(tooltipTriggerEl);
  });
}

// Back-to-top button
function initBackToTop() {
  const backButton = document.getElementById('back-to-top');
  if (!backButton) return;

  window.addEventListener('scroll', () => {
    backButton.classList.toggle('show', window.scrollY > 200);
  });

  backButton.addEventListener('click', () => {
    window.scrollTo({ top: 0, behavior: 'smooth' });
  });
}

// Contact form handling
function initContactForm() {
  const form = document.getElementById('contact-form');
  if (!form) return;

  form.addEventListener('submit', event => {
    event.preventDefault();
    if (form.checkValidity()) {
      // Example: show success alert
      alert('Thank you! Your message has been sent.');
      form.reset();
    } else {
      // Let HTML5 validation display messages
      form.reportValidity();
    }
  });
}

// Main initialization after DOM is loaded
document.addEventListener('DOMContentLoaded', () => {
  includeHTML('navbar', 'partials/menu.html');
  includeHTML('footer', 'partials/footer.html');
  includeHTML('contact-section', 'partials/contact-section.html');
  includeHTML('projects-section', 'partials/project-section.html');
  includeHTML('about-section', 'partials/about-section.html');
  initSmoothScroll();
  initTooltips();
  initBackToTop();
  initContactForm();
});
