# Interpretador de C, Grupo 09

## 1. A disciplina de Compiladores 1

Este projeto foi realizado por alunos da Universidade de Brasília durante a disciplina de Compiladores 1 (FGA0003), orientada pelo Prof. Dr. Sergio Antônio Andrade de Freitas.

Na disciplina, construímos um interpretador seguindo o pipeline léxico → sintático (com construção da AST) → semântico → interpretação recursiva da AST, usando Flex e Bison. Além do código, a proposta de trabalho pede que a equipe mantenha, durante todo o desenvolvimento, um registro das decisões técnicas, do planejamento das sprints e dos problemas e soluções. Como o professor só acessa o repositório na entrega final, é essa documentação que deixa o nosso trabalho visível e nos ajuda a defendê-lo na entrevista.

## 2. O projeto: Organizador de Compras no Mercado

Para o projeto ficar mais próximo de nós, decidimos juntos usar um tema do dia a dia.

Como estudo de caso para exercitar o núcleo da linguagem, escolhemos um **organizador de compras de mercado**: o programa lê os preços dos itens com `scanf` num laço, soma o total gasto e avisa se o valor passou do orçamento informado pelo usuário.

O interpretador cobre só o núcleo obrigatório da linguagem: tipos `int`/`float`/`char`, variáveis, expressões aritméticas/relacionais/lógicas com precedência, estruturas de controle (`if`/`else`, `while`, `for`), blocos com escopo, funções com parâmetros/retorno/recursão e entrada/saída básica (`printf`/`scanf`). Deixamos de fora, de propósito, ponteiros, structs, pré-processador e otimizações.

O exemplo completo de código e da saída esperada está em [Definição da Linguagem](/documentacao/definicao-da-linguagem.md), e os outros programas que usamos para validar o interpretador estão em [Estudos de Casos](/documentacao/estudos-de-casos.md).

## 3. Os Contribuidores

Somos alunos da Universidade de Brasília e formamos o Grupo 09.

<div class="team-container">

  <div class="team-card">
    <img src="docs/assets/Brenda_perfil.jpg" alt="Brenda Beatriz"/>
    <h3>Brenda Beatriz</h3>
    <p class="papel">Léxico e Sintático/AST; integração, testes e documentação; líder</p>
    <div class="social-links">
      <a href="https://github.com/Brwnds" target="_blank">GitHub</a>
    </div>
  </div>

 <div class="team-card">
    <img src="docs/assets/Eduardo_perfil.jpg" alt="Eduardo Ribeiro Xavier"/>
    <h3>Eduardo Ribeiro Xavier</h3>
    <p class="papel">Interpretação</p>
    <div class="social-links">
      <a href="https://github.com/EduardoRibeiroXavier" target="_blank">GitHub</a>
    </div>
  </div>

  <div class="team-card">
    <img src="docs/assets/Giovana_perfil.jpg" alt="Giovana Ferreira"/>
    <h3>Giovana Ferreira</h3>
    <p class="papel">Semântico: tabela de símbolos; integração (main.c)</p>
    <div class="social-links">
      <a href="https://github.com/gih7915" target="_blank">GitHub</a>
    </div>
  </div>

   <div class="team-card">
    <img src="docs/assets/Henrique_perfil.jpg" alt="Henrique Fontenelle"/>
    <h3>Henrique Fontenelle</h3>
    <p class="papel">Scrum Master: revisão e integração; suplente do líder</p>
    <div class="social-links">
      <a href="https://github.com/HenriqueFontenelle" target="_blank">GitHub</a>
    </div>
  </div>

  <div class="team-card">
    <img src="docs/assets/Mariana_perfil.jpg" alt="Mariana Martins"/>
    <h3>Mariana Martins</h3>
    <p class="papel">Semântico: tipos e compatibilidade</p>
    <div class="social-links">
      <a href="https://github.com/Marianamrts" target="_blank">GitHub</a>
    </div>
  </div>

</div>

<style>
.team-container {
  display: flex;
  flex-wrap: wrap;
  justify-content: center;
  gap: 25px;
  margin-top: 20px;
}

.team-card {
  width: 220px;
  background: linear-gradient(145deg, #f0f4f8, #d9e2ec);
  border-radius: 15px;
  padding: 20px;
  text-align: center;
  box-shadow: 0 6px 15px rgba(0,0,0,0.1);
  transition: transform 0.3s, box-shadow 0.3s;
  position: relative;
}

.team-card img {
  width: 120px;
  height: 120px;
  border-radius: 10px;
  object-fit: cover;
  margin-bottom: 15px;
}

.team-card h3 {
  font-size: 1.15em;
  color: #1f4e79;
  margin-bottom: 5px;
}

.team-card .papel {
  font-size: 0.85em;
  color: #334e68;
  margin: 0 0 10px;
}

.team-card .social-links {
  display: flex;
  justify-content: center;
  gap: 15px;
  opacity: 0;
  transform: translateY(20px);
  transition: opacity 0.4s, transform 0.4s;
}

.team-card .social-links a {
  text-decoration: none;
  font-weight: bold;
  color: #1f4e79;
  transition: color 0.2s;
}

.team-card .social-links a:hover {
  color: #0d2b4f;
}

.team-card:hover {
  transform: translateY(-10px);
  box-shadow: 0 12px 25px rgba(0,0,0,0.2);
}

.team-card:hover .social-links {
  opacity: 1;
  transform: translateY(0);
}
</style>

## Histórico de Versão

| Data | Versão | Descrição | Autor |
|------|--------|-----------|-------|
| 05/09/2026 | 1.0 | Criação da página de apresentação e equipe | Brenda |
| 23/11/2026 | 1.1 | Atualização da página de apresentação da equipe | Mariana |
| 26/11/2026 | 1.2 | Revisão do texto | Brenda |
