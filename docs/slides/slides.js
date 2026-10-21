/**
 * slides.js - Motor de Apresentação e Exportação de Slides
 * Suporta navegação por teclado, escala dinâmica proporcional 16:9,
 * hash URL, modo visão geral (grid) e impressão direta para PDF.
 */

(function () {
  'use strict';

  // Configurações Globais (Referência 16:9 Widescreen)
  const CONFIG = {
    slideWidth: 1280,
    slideHeight: 720,
    scalePadding: 0.96, // Margem de segurança de escala na tela
  };

  // Elementos DOM
  const container = document.getElementById('slides-container');
  const slides = Array.from(document.querySelectorAll('.slide'));
  const counterEl = document.getElementById('current-slide-index');
  const totalEl = document.getElementById('total-slides-count');
  const progressBar = document.getElementById('progress-bar');
  const helpModal = document.getElementById('help-modal');

  let currentSlide = 0;
  let isOverview = false;

  // 1. Inicialização
  function init() {
    if (!slides.length) return;

    if (totalEl) {
      totalEl.textContent = String(slides.length).padStart(2, '0');
    }

    // Identificar slide inicial via URL Hash (#3)
    const hashIndex = getSlideFromHash();
    goToSlide(hashIndex, false);

    // Configurar Escala
    updateViewportScale();
    window.addEventListener('resize', updateViewportScale);

    // Código em C com destaque de sintaxe e vídeos
    highlightCode();
    setupVideos();

    // Event Listeners
    setupKeyboard();
    setupButtons();
    setupOverviewClick();

    window.addEventListener('hashchange', () => {
      const idx = getSlideFromHash();
      if (idx !== currentSlide) {
        goToSlide(idx, false);
      }
    });

    console.log(`[Slides Engine] Inicializado com ${slides.length} slides.`);
  }

  // 2. Escala Dinâmica (mantém 16:9 nítido em qualquer monitor)
  function updateViewportScale() {
    if (isOverview || window.matchMedia('print').matches) {
      document.documentElement.style.removeProperty('--viewport-scale');
      return;
    }

    const vw = window.innerWidth;
    const vh = window.innerHeight;

    const scaleX = (vw * CONFIG.scalePadding) / CONFIG.slideWidth;
    const scaleY = (vh * CONFIG.scalePadding) / CONFIG.slideHeight;
    const scale = Math.min(scaleX, scaleY);

    document.documentElement.style.setProperty('--viewport-scale', scale.toFixed(4));
  }

  // 3. Navegação entre Slides
  function goToSlide(index, updateHash = true) {
    if (index < 0) index = 0;
    if (index >= slides.length) index = slides.length - 1;

    slides.forEach((slide, i) => {
      slide.classList.toggle('active', i === index);
    });

    currentSlide = index;

    // Vídeo de um slide que saiu da tela não continua tocando
    slides.forEach((slide, i) => {
      if (i !== index) {
        slide.querySelectorAll('video').forEach((v) => v.pause());
      }
    });

    // Atualizar contador e barra
    if (counterEl) {
      counterEl.textContent = String(currentSlide + 1).padStart(2, '0');
    }

    if (progressBar) {
      const pct = slides.length > 1 ? (currentSlide / (slides.length - 1)) * 100 : 100;
      progressBar.style.width = `${pct}%`;
    }

    // Atualizar número no rodapé de cada slide
    slides.forEach((slide, i) => {
      const numSpan = slide.querySelector('.footer-slide-num');
      if (numSpan) {
        numSpan.textContent = `${String(i + 1).padStart(2, '0')} / ${String(slides.length).padStart(2, '0')}`;
      }
    });

    if (updateHash) {
      history.replaceState(null, '', `#${currentSlide + 1}`);
    }
  }

  function nextSlide() {
    if (currentSlide < slides.length - 1) {
      goToSlide(currentSlide + 1);
    }
  }

  function prevSlide() {
    if (currentSlide > 0) {
      goToSlide(currentSlide - 1);
    }
  }

  function getSlideFromHash() {
    const hash = window.location.hash.replace('#', '');
    const num = parseInt(hash, 10);
    if (!isNaN(num) && num >= 1 && num <= slides.length) {
      return num - 1;
    }
    return 0;
  }

  // 4. Modos Especiais: Visão Geral (Overview) e Tela Cheia
  function toggleOverview() {
    isOverview = !isOverview;
    document.body.classList.toggle('overview-mode', isOverview);
    updateViewportScale();

    if (!isOverview) {
      goToSlide(currentSlide);
      setTimeout(updateViewportScale, 50);
    }
  }

  function toggleFullscreen() {
    if (!document.fullscreenElement) {
      document.documentElement.requestFullscreen().catch((err) => {
        console.warn('Erro ao ativar tela cheia:', err);
      });
    } else {
      document.exitFullscreen().catch((err) => {
        console.warn('Erro ao sair da tela cheia:', err);
      });
    }
  }

  function toggleTheme() {
    document.body.classList.toggle('theme-light');
  }

  function toggleHelp() {
    if (helpModal) {
      helpModal.classList.toggle('open');
    }
  }

  // 5. Configurar Teclado
  function setupKeyboard() {
    window.addEventListener('keydown', (e) => {
      // Com o foco no vídeo, Espaço e Enter são do player (tocar e pausar)
      const alvo = e.target && e.target.tagName;
      if ((alvo === 'VIDEO' || alvo === 'BUTTON') && (e.key === ' ' || e.key === 'Enter')) {
        return;
      }

      // Se modal de ajuda estiver aberto, ESC ou ? fecham
      if (helpModal && helpModal.classList.contains('open')) {
        if (e.key === 'Escape' || e.key === '?') {
          toggleHelp();
          e.preventDefault();
        }
        return;
      }

      switch (e.key) {
        case 'ArrowRight':
        case 'ArrowDown':
        case 'PageDown':
        case ' ': // Espaço
        case 'Enter':
          if (!isOverview) nextSlide();
          e.preventDefault();
          break;

        case 'ArrowLeft':
        case 'ArrowUp':
        case 'PageUp':
        case 'Backspace':
          if (!isOverview) prevSlide();
          e.preventDefault();
          break;

        case 'Home':
          goToSlide(0);
          e.preventDefault();
          break;

        case 'End':
          goToSlide(slides.length - 1);
          e.preventDefault();
          break;

        case 'f':
        case 'F':
          toggleFullscreen();
          e.preventDefault();
          break;

        case 'o':
        case 'O':
          toggleOverview();
          e.preventDefault();
          break;

        case 't':
        case 'T':
          toggleTheme();
          e.preventDefault();
          break;

        case 'p':
        case 'P':
          if (e.ctrlKey || e.metaKey) {
            // Deixa atalho padrão do navegador para imprimir
            return;
          }
          window.print();
          e.preventDefault();
          break;

        case '?':
          toggleHelp();
          e.preventDefault();
          break;

        case 'Escape':
          if (isOverview) {
            toggleOverview();
            e.preventDefault();
          }
          break;
      }
    });
  }

  // 6. Botões da Barra Flutuante
  function setupButtons() {
    const btnPrev = document.getElementById('btn-prev');
    const btnNext = document.getElementById('btn-next');
    const btnOverview = document.getElementById('btn-overview');
    const btnFullscreen = document.getElementById('btn-fullscreen');
    const btnTheme = document.getElementById('btn-theme');
    const btnPrint = document.getElementById('btn-print');
    const btnHelp = document.getElementById('btn-help');
    const btnCloseHelp = document.getElementById('btn-close-help');

    if (btnPrev) btnPrev.addEventListener('click', prevSlide);
    if (btnNext) btnNext.addEventListener('click', nextSlide);
    if (btnOverview) btnOverview.addEventListener('click', toggleOverview);
    if (btnFullscreen) btnFullscreen.addEventListener('click', toggleFullscreen);
    if (btnTheme) btnTheme.addEventListener('click', toggleTheme);
    if (btnPrint) btnPrint.addEventListener('click', () => window.print());
    if (btnHelp) btnHelp.addEventListener('click', toggleHelp);
    if (btnCloseHelp) btnCloseHelp.addEventListener('click', toggleHelp);

    if (helpModal) {
      helpModal.addEventListener('click', (e) => {
        if (e.target === helpModal) toggleHelp();
      });
    }
  }

  // 7. Clique nos Slides no modo Overview
  function setupOverviewClick() {
    slides.forEach((slide, index) => {
      slide.addEventListener('click', () => {
        if (isOverview) {
          goToSlide(index);
          toggleOverview();
        }
      });
    });
  }

  // 8. Destaque de sintaxe em C
  // Os blocos <pre class="code-body" data-lang="c"> são escritos como texto
  // puro no HTML; aqui cada trecho é classificado (comentário, texto, diretiva,
  // palavra-chave, número, chamada de função) e envolvido no <span> da cor.
  const C_KEYWORDS = 'static|int|float|void|unsigned|char|if|else|for|while|return|const';
  const C_TOKEN = new RegExp(
    '(\\/\\/[^\\n]*|\\/\\*[\\s\\S]*?\\*\\/)' +          // 1 comentário
    '|("(?:[^"\\\\]|\\\\.)*"|\'(?:[^\'\\\\]|\\\\.)*\')' + // 2 texto ou caractere
    '|(#\\s*[a-z]+)' +                                          // 3 diretiva
    '|\\b(' + C_KEYWORDS + ')\\b' +                             // 4 palavra-chave
    '|\\b(\\d+(?:\\.\\d+)?f?)\\b' +                             // 5 número
    '|\\b([A-Za-z_]\\w*)(?=\\s*\\()',                             // 6 chamada
    'g'
  );
  const C_CLASSES = [null, 'c-comment', 'c-str', 'c-kw', 'c-kw', 'c-num', 'c-fn'];

  function escapeHtml(texto) {
    return texto.replace(/&/g, '&amp;').replace(/</g, '&lt;').replace(/>/g, '&gt;');
  }

  function highlightC(fonte) {
    let saida = '';
    let ultimo = 0;
    let m;
    C_TOKEN.lastIndex = 0;
    while ((m = C_TOKEN.exec(fonte)) !== null) {
      let grupo = 1;
      while (grupo < C_CLASSES.length && m[grupo] === undefined) grupo++;
      saida += escapeHtml(fonte.slice(ultimo, m.index));
      saida += '<span class="' + C_CLASSES[grupo] + '">' + escapeHtml(m[0]) + '</span>';
      ultimo = C_TOKEN.lastIndex;
    }
    return saida + escapeHtml(fonte.slice(ultimo));
  }

  function highlightCode() {
    document.querySelectorAll('pre.code-body[data-lang="c"] code').forEach((bloco) => {
      bloco.innerHTML = highlightC(bloco.textContent);
    });
  }

  // 9. Vídeos: aviso quando o arquivo .mp4 não está ao lado do index.html
  function setupVideos() {
    document.querySelectorAll('.video-frame video').forEach((video) => {
      const marcarAusente = () => video.closest('.video-frame').classList.add('video-missing');
      video.addEventListener('error', marcarAusente);
      if (video.error) marcarAusente();
    });
  }

  // Disparar no carregamento
  if (document.readyState === 'loading') {
    document.addEventListener('DOMContentLoaded', init);
  } else {
    init();
  }
})();

