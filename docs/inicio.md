<div class="home">

<section class="home-hero">
  <nav class="home-nav">
    <span class="home-nav-marca"><b>&lt;/&gt;</b> interpretador-c <small>grupo 09</small></span>
    <span class="home-nav-links">
      <a href="#/documentacao/apresentacao">documentação</a>
      <a href="#/documentacao/estudos-de-casos">estudos de caso</a>
      <a href="https://github.com/CompiladoresGrupo09/CompiladoresGrupo-09" target="_blank">github</a>
    </span>
  </nav>
  <div class="home-hero-grade">
    <div class="home-hero-texto">
      <p class="home-comentario">/* Compiladores 1 · UnB 2026.2 · Grupo 09 */</p>
      <h1 class="home-title">Interpretador<br/>de <em>C</em><span class="home-cursor"></span></h1>
      <p class="home-lead">Construímos com Flex e Bison um interpretador que lê um programa em C, analisa cada parte e executa o resultado, avisando com clareza quando algo está errado.</p>
      <div class="home-actions">
        <a class="home-btn home-btn-primary" href="#/documentacao/apresentacao">Abrir a documentação <b>→</b></a>
        <a class="home-btn home-btn-cmd" href="#/documentacao/estudos-de-casos"><b>$</b> ./interpretador organizador_compras.c</a>
      </div>
    </div>
    <div class="home-painel">
      <div class="home-painel-topo"><span></span><span></span><span></span><em>uma linha do organizador de compras</em></div>
      <div class="home-painel-fonte"><span class="sx-id">total</span> = <span class="sx-id">total</span> + <span class="sx-id">preco</span>;</div>
      <div class="home-fase">
        <p class="home-fase-nome"><b>01</b> léxico <i>tokens</i></p>
        <div class="home-tokens"><span><i>ID</i>total</span><span class="tk-s"><i>SIMBOLO</i>=</span><span><i>ID</i>total</span><span class="tk-s"><i>SIMBOLO</i>+</span><span><i>ID</i>preco</span><span class="tk-s"><i>SIMBOLO</i>;</span></div>
      </div>
      <div class="home-fase">
        <p class="home-fase-nome"><b>02</b> sintático <i>AST</i></p>
<pre class="home-ast"><span class="sx-kw">BINOP</span> <span class="sx-str">'='</span>
├─ <span class="sx-tp">ID</span> <span class="sx-str">'total'</span>
└─ <span class="sx-kw">BINOP</span> <span class="sx-str">'+'</span>
   ├─ <span class="sx-tp">ID</span> <span class="sx-str">'total'</span>
   └─ <span class="sx-tp">ID</span> <span class="sx-str">'preco'</span></pre>
      </div>
      <div class="home-fase">
        <p class="home-fase-nome"><b>03</b> semântico <i>tipos</i></p>
        <p class="home-fase-texto"><span class="sx-id">total</span> : <span class="sx-tp">float</span> · <span class="sx-id">preco</span> : <span class="sx-tp">float</span> <span class="home-ok">✓ compatível</span></p>
      </div>
      <div class="home-fase">
        <p class="home-fase-nome"><b>04</b> execução <i>saída</i></p>
        <p class="home-fase-texto home-saida">Total gasto: <span class="sx-num">120.000000</span></p>
      </div>
    </div>
  </div>
  <button class="home-rolar" onclick="window.scrollTo({ top: window.innerHeight, behavior: 'smooth' })">// role para conhecer o projeto <i>↓</i></button>
</section>

<div class="home-corpo">

<section class="home-stats">
  <div class="home-stat"><strong>5</strong><span>fases no pipeline</span></div>
  <div class="home-stat"><strong>88</strong><span>testes automatizados</span></div>
  <div class="home-stat"><strong>4</strong><span>estudos de caso</span></div>
</section>

<section class="home-section">
  <p class="home-kicker">Do código-fonte ao resultado</p>
  <h2 class="home-h2">Como o interpretador funciona</h2>
  <div class="home-steps">
    <div class="home-step"><span class="home-step-n">01</span><h3>Léxico</h3><p>O scanner (Flex) separa o texto em tokens e guarda a linha de cada um.</p></div>
    <div class="home-step"><span class="home-step-n">02</span><h3>Sintático</h3><p>O parser (Bison) confere a gramática e já monta a árvore sintática (AST).</p></div>
    <div class="home-step"><span class="home-step-n">03</span><h3>Semântico</h3><p>A tabela de símbolos confere declarações, escopos, tipos e chamadas de função.</p></div>
    <div class="home-step"><span class="home-step-n">04</span><h3>Execução</h3><p>Percorremos a AST e executamos cada nó: laços, funções, recursão, <code>scanf</code> e <code>printf</code>.</p></div>
    <div class="home-step"><span class="home-step-n">05</span><h3>Resultado</h3><p>A saída do programa e um relatório com os erros de cada fase.</p></div>
  </div>
</section>

<section class="home-section home-case">
  <div class="home-case-text">
    <p class="home-kicker">Estudo de caso principal</p>
    <h2 class="home-h2">Organizador de compras no mercado</h2>
    <p>Para o projeto ficar perto do nosso dia a dia, escolhemos um problema que todo mundo conhece: chegar no caixa e descobrir que passou do orçamento.</p>
    <p>O programa lê o orçamento e o preço de cada item, soma o total e avisa se o valor passou do limite. Ele usa só o núcleo da linguagem e foi o primeiro programa completo que o nosso interpretador executou de ponta a ponta.</p>
    <ul class="home-tags">
      <li><code>float</code> e <code>int</code></li>
      <li><code>while</code></li>
      <li><code>if</code> / <code>else</code></li>
      <li><code>scanf</code></li>
      <li><code>printf</code></li>
    </ul>
    <a class="home-btn home-btn-primary" href="#/documentacao/definicao-da-linguagem">Ver o código completo</a>
  </div>
  <div class="home-terminal">
    <div class="home-terminal-bar"><span></span><span></span><span></span><em>organizador_compras.c</em></div>
<pre>$ ./interpretador organizador_compras.c
Digite o orcamento disponivel: <b>100</b>
Digite o preco de cada item (digite -1 para finalizar):
Preco do item: <b>30</b>
Preco do item: <b>40</b>
Preco do item: <b>50</b>
Preco do item: <b>-1</b>
Itens registrados: 3
Total gasto: 120.000000
<i>Atencao: voce ultrapassou o orcamento!</i>
&#8203;
==== Relatorio de erros ====
Lexica     0 erro(s)
Sintatica  0 erro(s)
Semantica  0 erro(s)
Execucao   0 erro(s)</pre>
  </div>
</section>

<section class="home-section">
  <p class="home-kicker">Validação final</p>
  <h2 class="home-h2">Um estudo de caso por integrante</h2>
  <p class="home-sub">Cada um de nós escreveu um programa que usa uma parte diferente da linguagem. Juntos, eles passam por todo o núcleo, e foi assim que encontramos e corrigimos os últimos defeitos.</p>
  <div class="home-cards">
    <a class="home-card" href="#/documentacao/estudos-de-casos"><span class="home-card-icon">01</span><h3>Boletim escolar</h3><p><code>float</code>, <code>if</code>/<code>else</code> aninhado e função com retorno <code>float</code>.</p><span class="home-card-who">Mariana</span></a>
    <a class="home-card" href="#/documentacao/estudos-de-casos"><span class="home-card-icon">02</span><h3>Caixa eletrônico</h3><p>Menu com <code>while</code>, variáveis globais e funções <code>void</code>.</p><span class="home-card-who">Giovana</span></a>
    <a class="home-card" href="#/documentacao/estudos-de-casos"><span class="home-card-icon">03</span><h3>Calculadora de números</h3><p>Recursão (MDC e fatorial), <code>for</code> aninhado e o operador <code>%</code>.</p><span class="home-card-who">Eduardo</span></a>
    <a class="home-card" href="#/documentacao/estudos-de-casos"><span class="home-card-icon">04</span><h3>Controle de temperatura</h3><p><code>char</code> e <code>%c</code>, sentinela <code>-1</code>, escopo e <i>shadowing</i>.</p><span class="home-card-who">Brenda</span></a>
  </div>
</section>

<section class="home-section">
  <h2 class="home-h2">Equipe</h2>
  <div class="home-team">
    <a class="home-member" href="https://github.com/Brwnds" target="_blank"><img src="docs/assets/Brenda_perfil.jpg" alt="Brenda Beatriz"/><strong>Brenda Beatriz</strong></a>
    <a class="home-member" href="https://github.com/EduardoRibeiroXavier" target="_blank"><img src="docs/assets/Eduardo_perfil.jpg" alt="Eduardo Ribeiro Xavier"/><strong>Eduardo Ribeiro Xavier</strong></a>
    <a class="home-member" href="https://github.com/gih7915" target="_blank"><img src="docs/assets/Giovana_perfil.jpg" alt="Giovana Ferreira"/><strong>Giovana Ferreira</strong></a>
    <a class="home-member" href="https://github.com/HenriqueFontenelle" target="_blank"><img src="docs/assets/Henrique_perfil.jpg" alt="Henrique Fontenelle"/><strong>Henrique Fontenelle</strong></a>
    <a class="home-member" href="https://github.com/Marianamrts" target="_blank"><img src="docs/assets/Mariana_perfil.jpg" alt="Mariana Martins"/><strong>Mariana Martins</strong></a>
  </div>
</section>

<section class="home-section">
  <p class="home-kicker">Documentação</p>
  <h2 class="home-h2">Tudo o que registramos</h2>
  <div class="home-docs">
    <a href="#/documentacao/apresentacao"><span>01</span>Apresentação e equipe</a>
    <a href="#/README"><span>02</span>Visão geral do projeto</a>
    <a href="#/documentacao/definicao-da-linguagem"><span>03</span>Definição da linguagem</a>
    <a href="#/documentacao/gramatica"><span>04</span>Gramática</a>
    <a href="#/documentacao/estudos-de-casos"><span>05</span>Estudos de caso</a>
    <a href="#/documentacao/demonstracao"><span>06</span>Roteiro de demonstração</a>
    <a href="#/documentacao/decisoes-tecnicas"><span>07</span>Decisões técnicas</a>
    <a href="#/documentacao/planejamento-sprints"><span>08</span>Planejamento das sprints</a>
    <a href="#/documentacao/problemas-e-solucoes"><span>09</span>Problemas e soluções</a>
    <a href="#/documentacao/glossario"><span>10</span>Glossário</a>
  </div>
</section>

</div>

</div>
